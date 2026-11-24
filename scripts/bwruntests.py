#!/usr/bin/env python3

# Copyright 2020 ETH Zurich and University of Bologna
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Run shell commands listed in a file separated by newlines in a parallel
# fashion. If requested the results (tuples consisting of command, stdout,
# stderr and returncode) will be gathered in a junit.xml file. There a few
# knobs to tune the number of spawned processes and the junit.xml formatting.

# Author: Robert Balas (balasr@iis.ee.ethz.ch)

import argparse
from subprocess import (Popen, TimeoutExpired,
                        CalledProcessError, PIPE)
from threading import Lock
import shlex
import sys
import signal
import os
import multiprocessing
import errno
import pprint
import time


runtest = argparse.ArgumentParser(
    prog='bwruntests',
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""Run PULP tests in parallel""",
    epilog="""
Test_file needs to be either a .yaml file (set the --yaml switch)
which looks like this:

mytests.yml
[...]
parallel_bare_tests: # name of the test set
  parMatrixMul8:     # name of the test
    path: ./parallel_bare_tests/parMatrixMul8 # path to the test's folder
    command: make clean all run # command to run in the test's folder
[...]

or

Test_file needs to be a list of commands to be executed. Each line corresponds
to a single command and a test

commands.f
[...]
make -C ./ml_tests/mlGrad clean all run
make -C ./ml_tests/mlDct clean all run
[...]

Example:
bwruntests.py --proc-verbose -v \\
    --report-junit -t 3600 --yaml \\
    -o simplified-runtime.xml runtime-tests.yaml

This Runs a set of tests defined in runtime-tests.yaml and dumps the
resulting junit.xml into simplified-runtime.xml. The --proc-verbose
scripts makes sure to print the stdout of each process to the shell. To
prevent a broken process from running forever, a maximum timeout of 3600
seconds was set. For debugging purposes we enabled -v (--verbose) which
shows the full set of commands being run.""")

runtest.version = '0.2'

runtest.add_argument('test_file', type=str,
                     help='file defining tests to be run')
runtest.add_argument('--version', action='version',
                     version='%(prog)s ' + runtest.version)
runtest.add_argument('-p', '--max-procs', type=int,
                     default=multiprocessing.cpu_count(),
                     help="""Number of parallel
                     processes used to run test.
                     Default is number of cpu cores.""")
runtest.add_argument('-t', '--timeout', type=float,
                     default=None,
                     help="""Timeout for all processes in seconds""")
runtest.add_argument('-v', '--verbose', action='store_true',
                     help="""Enable verbose output""")
runtest.add_argument('-s', '--proc-verbose', action='store_true',
                     help="""Write processes' stdout and stderr to shell stdout
                     after they terminate""")
runtest.add_argument('--report-junit', action='store_true',
                     help="""Generate a junit report""")
runtest.add_argument('--disable-junit-pp', action='store_true',
                     help="""Disable pretty print of junit report""")
runtest.add_argument('--disable-results-pp', action='store_true',
                     help="""Disable printing test results""")
runtest.add_argument('-y,', '--yaml', action='store_true',
                     help="""Read tests from yaml file instead of executing
                     from a list of commands""")
runtest.add_argument('-o,', '--output', type=str,
                     help="""Write junit.xml to file instead of stdout""")
stdout_lock = Lock()


class FinishedProcess(object):
    """A process that has finished running.
    """
    def __init__(self, name, cwd, args, returncode,
                 stdout=None, stderr=None, time=None):
        self.name = name
        self.cwd = cwd
        self.args = args
        self.returncode = returncode
        self.stdout = stdout
        self.stderr = stderr
        self.time = time

    def __repr__(self):
        args = ['name={!r}'.format(self.name)]
        args += ['cwd={!r}'.format(self.cwd)]
        args += ['args={!r}'.format(self.args),
                 'returncode={!r}'.format(self.returncode)]
        if self.stdout is not None:
            args.append('stdout={!r}'.format(self.stdout))
        if self.stderr is not None:
            args.append('stderr={!r}'.format(self.stderr))
        if self.time is not None:
            args.append('time={!r}'.format(self.time))
        return "{}({})".format(type(self).__name__, ', '.join(args))


def fork(name, cwd, *popenargs, check=False, shell=True,
         **kwargs):
    """Run subprocess and return process args, error code, stdout and stderr
    """

    def proc_out(cwd, stdout, stderr):
        print('cwd={}'.format(cwd))
        print('stdout=')
        print(stdout.decode('utf-8'))
        print('stderr=')
        print(stderr.decode('utf-8'))

    kwargs['stdout'] = PIPE
    kwargs['stderr'] = PIPE

    with Popen(*popenargs, preexec_fn=os.setpgrp, cwd=cwd,
               **kwargs) as process:
        try:
            # Child and parent are racing for setting/using the pgid so we have
            # to set it in both processes. See glib manual.
            try:
                os.setpgid(process.pid, process.pid)
            except OSError as e:
                if e.errno != errno.EACCES:
                    raise
            # measure runtime
            start = time.time()
            stdout, stderr = process.communicate(input, timeout=args.timeout)
        except TimeoutExpired:
            pgid = os.getpgid(process.pid)
            os.killpg(pgid, signal.SIGKILL)
            # process.kill() will only kill the immediate child but not its
            # forks. This won't work since our commands will create a few forks
            # (make -> vsim -> etc). We need to make a process group and kill
            # that
            stdout, stderr = process.communicate()
            timeoutmsg = 'TIMEOUT after {:f}s'.format(args.timeout)

            if args.proc_verbose:
                stdout_lock.acquire()
                print(name)
                print(timeoutmsg)
                proc_out(cwd, stdout, stderr)
                stdout_lock.release()

            return FinishedProcess(name, cwd, process.args, 1,
                                   stdout.decode('utf-8'),
                                   timeoutmsg + '\n'
                                   + stderr.decode('utf-8'),
                                   time.time() - start)
        # Including KeyboardInterrupt, communicate handled that.
        except:  # noqa: E722
            pgid = os.getpgid(process.pid)
            os.killpg(pgid, signal.SIGKILL)
            # We don't call process.wait() as .__exit__ does that for us.
            raise
        retcode = process.poll()
        if check and retcode:
            raise CalledProcessError(retcode, process.args,
                                     output=stdout, stderr=stderr)
        if args.proc_verbose:
            stdout_lock.acquire()
            print(name)
            proc_out(cwd, stdout, stderr)
            stdout_lock.release()

    return FinishedProcess(name, cwd, process.args, retcode,
                           stdout.decode('utf-8'),
                           stderr.decode('utf-8'),
                           time.time() - start)


if __name__ == '__main__':
    args = runtest.parse_args()
    pp = pprint.PrettyPrinter(indent=4)

    # lazy importing so that we can work without junit_xml
    if args.report_junit:
        try:
            from junit_xml import TestSuite, TestCase
        except ImportError:
            print("""Error: The --report-junit option requires
the junit_xml library which is not installed.""",
                  file=sys.stderr)
            exit(1)

    # lazy import PrettyTable for displaying results
    if not(args.disable_results_pp):
        try:
            from prettytable import PrettyTable
        except ImportError:
            print("""Warning: Displaying results requires the PrettyTable
library which is not installed""")

    tests = []  # list of tuple (testname, working dir, command)

    # load tests (yaml or command list)
    if args.yaml:
        try:
            import yaml
        except ImportError:
            print("""Error: The --yaml option requires
the pyyaml library which is not installed.""",
                  file=sys.stderr)
            exit(1)
        with open(args.test_file) as f:
            testyaml = yaml.load(f, Loader=yaml.Loader)
            platform = os.environ.get("platform", "rtl")
            for testsetname, testv in testyaml.items():
                for testname, insn in testv.items():
                    platforms = insn.get("platforms")
                    if platforms is not None and platform not in platforms:
                        continue
                    cmd = shlex.split(os.path.expandvars(insn['command']))
                    cwd = insn['path']
                    tests.append((testsetname + ':' + testname, cwd, cmd))
            if args.verbose:
                pp.pprint(tests)
    else:  # (command list)
        with open(args.test_file) as f:
            testnames = list(map(str.rstrip, f))
            shellcmds = [shlex.split(e) for e in testnames]
            cwds = ['./' for e in testnames]
            tests = list(zip(testnames, cwds, shellcmds))
            if args.verbose:
                print('Tests which we are running:')
                pp.pprint(tests)
                pp.pprint(shellcmds)

    # Spawning process pool
    # Disable signals to prevent race. Child processes inherit SIGINT handler
    original_sigint_handler = signal.signal(signal.SIGINT, signal.SIG_IGN)
    pool = multiprocessing.Pool(processes=args.max_procs)
    # Restore SIGINT handler
    signal.signal(signal.SIGINT, original_sigint_handler)
    try:
        procresults = pool.starmap(fork, tests)
    except KeyboardInterrupt:
        print("\nTerminating bwruntest.py")
        pool.terminate()
        pool.join()
        exit(1)

    if args.verbose:
        pp.pprint(procresults)
    pool.close()
    pool.join()

    # Generate junit.xml file. Junit.xml differentiates between failure and
    # errors but we treat everything as errors.
    if args.report_junit:
        testcases = []
        for p in procresults:
            # we can either expect p.name = testsetname:testname
            # or p.name = testname
            testcase = TestCase(p.name,
                                classname=((p.name).split(':'))[0],
                                stdout=p.stdout,
                                stderr=p.stderr,
                                elapsed_sec=p.time)
            if p.returncode != 0:
                testcase.add_failure_info(p.stderr)
            testcases.append(testcase)

        testsuite = TestSuite('bwruntests', testcases)
        if args.output:
            with open(args.output, 'w') as f:
                TestSuite.to_file(f, [testsuite],
                                  prettyprint=not(args.disable_junit_pp))
        else:
            print(TestSuite.to_xml_string([testsuite],
                                          prettyprint=(args.disable_junit_pp)))

    # print summary of test results
    if not(args.disable_results_pp):
        testcount = sum(1 for x in tests)
        testfailcount = sum(1 for p in procresults if p.returncode != 0)
        testpassedcount = testcount - testfailcount
        status = "CI_FAILURE" if testfailcount > 0 else "CI_SUCCESS"
        resulttable = PrettyTable(['test', 'time', 'passed/total'])
        resulttable.align['test'] = "l"
        resulttable.add_row(['bwruntest', '', '{0:d}/{1:d}'.
                             format(testpassedcount, testcount)])
        for p in procresults:
            testpassed = 1 if p.returncode == 0 else 0
            testname = p.name
            resulttable.add_row([testname,
                                 '{0:.2f}s'.format(p.time),
                                 '{0:d}/{1:d}'.format(testpassed, 1)])
        resulttable.add_row([status, '', ''])
        print(resulttable)
