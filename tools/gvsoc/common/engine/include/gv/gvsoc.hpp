/*
 * Copyright (C) 2021 GreenWaves Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once


#include <string>

namespace gv {


    /**
     * IO request status.
     */
    enum Io_request_status {
        // Request was successfull
        Io_request_ok,
        // Request failed
        Io_request_ko
    };


    /**
     * Class used to represent a memory-mapped request.
     *
     * Used in all methods related to IO requests.
     */
    class Io_request
    {
    public:
        Io_request() : data(NULL), size(0), is_write(false), addr(0) {}

        // Pointer to the area containing the data
        uint8_t *data;
        // Size in bytes of the access
        size_t size;
        // True if the request is a write
        bool is_write;
        // Address of the access
        uint64_t addr;
        // Status of the access
        Io_request_status retval;

        // Reserved
        void *handle;
    };


    /**
     * Class used to represent IO bindings.
     *
     * This class defines all the methods that the external C++ can call to interact with
     * GVSOC for what concerns IO requests.
     */
    class Io_binding
    {
    public:
        /**
         * Inject a memory-mapped access.
         *
         * This operation is asynchronous. The request can be granted and replied by callbacks
         * executed afterwards.
         *
         * @param req The IO request describing the memory-mapped access.
         */
        virtual void access(Io_request *req) = 0;

        /**
         * Grant a memory-mapped access.
         *
         * This can be used to grant an incoming request. This can be used to notify the
         * initiator of the request that the request is accepted and that another one can be sent.
         * The initiator will usually use this information to pipeline several requests.
         * Not granting a request will prevent the initiator from sending another request.
         * Note that this step is optional, the request can be directly replied.
         *
         * @param req The IO request describing the memory-mapped access.
         */
        virtual void grant(Io_request *req) = 0;

        /**
         * Reply to a memory-mapped access.
         *
         * This can be used to reply to an incoming request. This can be used to notify the
         * initiator of the request that the request is over. The request contains the status
         * of the request and in case of a read command, the request contains the read data.
         *
         * @param req The IO request describing the memory-mapped access.
         */
        virtual void reply(Io_request *req) = 0;
    };


    /**
     * Class required for binding.
     *
     * When the external C++ code creates a binding, it must implement all the methods of this class
     * to properly interact with GVSOC for what concerns IO requests.
     */
    class Io_user
    {
    public:
        /**
         * Called by GVSOC to inject a memory-mapped access.
         *
         * This operation is asynchronous. This method must return immediately and the request must be granted and/or 
         * replied afterwards.
         * No GVSOC API can be called from this callback.
         *
         * @param req The IO request describing the memory-mapped access.
         */
        virtual void access(Io_request *req) = 0;

        /**
         * Called by GVSOC tp grant a memory-mapped access.
         *
         * This can be used to grant an incoming request. This can be used to notify the
         * initiator of the request that the request is accepted and that another one can be sent.
         * The initiator will usually use this information to pipeline several requests.
         * Not granting a request will prevent the initiator from sending another request.
         * Note that this step is optional, the request can be directly replied.
         * No GVSOC API can be called from this callback.
         *
         * @param req The IO request describing the memory-mapped access.
         */
        virtual void grant(Io_request *req) = 0;

        /**
         * Called by GVSOC to reply to a memory-mapped access.
         *
         * This can be used to reply to an incoming request. This can be used to notify the
         * initiator of the request that the request is over. The request contains the status
         * of the request and in case of a read command, the request contains the read data.
         * No GVSOC API can be called from this callback.
         *
         * @param req The IO request describing the memory-mapped access.
         */
        virtual void reply(Io_request *req) = 0;
    };


    /**
     * GVSOC interface for IO requests
     *
     * Gather all the methods which can be called to bind the simulated system with external C++ code
     * so that requests can be exchanged in both directions.
     */
    class Io
    {
    public:
        /**
         * Create a binding to the simulated system
         *
         * This creates a binding so that the external C++ code can exchange IO requests in both direction
         * with the simulated system. This is based on the same set of methods which must be implemented on
         * both sides.
         *
         * @param user A pointer to the caller class instance which will be called for all IO callbacks. This caller
         *             must implement all the methods defined in class Io_user
         * @param comp_name The name of the component where to connect.
         * @param itf_name The name of the component interface where to connect.
         *
         * @return A class instance which can be used to inject IO requests, or grant and reply to incoming requests.
         */
        virtual Io_binding *io_bind(Io_user *user, std::string comp_name, std::string itf_name) = 0;

    };


    /**
     * GVSOC interface
     *
     * Gather all the methods which can be called to control GVSOC execution and other features
     */
    class Gvsoc : public Io
    {
    public:
        /**
         * Open a GVSOC configuration
         *
         * This instantiates the system described in the specified configuration file.
         * Once this operation is done, some bindings can be created to interact with this system
         * and GVSOC execution can be started.
         *
         * @param config_path The path to the configuration file.
         */
        virtual void open(std::string config_path) = 0;

        /**
         * Close a GVSOC configuration
         *
         * This closes the currently opened configuration so that another one can be
         * opened.
         *
         */
        virtual void close() = 0;

        /**
         * Run execution
         *
         * This starts execution in a separate thread and immediately returns so that
         * the caller can do something else while GVSOC is running.
         *
         */
        virtual void run() = 0;

        /**
         * Stop execution
         *
         * This stops execution. Once it is called, nothing is executed in GVSOC
         * until execution is resumed.
         *
         * @returns The timestamp where the execution stopped.
         */
        virtual int64_t stop() = 0;

        /**
         * Step execution
         *
         * Start execution and run until the specified duration is reached. Then
         * execution is stopped and nothing is executed until it is resumed.
         *
         * @param duration The amount of time in picoseconds during which GVSOC should execute.
         *
         * @returns The timestamp where the execution will stop after the duration is reached.
         */
        virtual int64_t step(int64_t duration) = 0;
    };


    /**
     * Instantiate GVSOC
     *
     * This has to be called to instantiate an object representing GVSOC which can
     * then be used to control GVSOC execution and other features like bindings.
     *
     * @returns A pointer to the allocated GVSOC objet.
     */
    Gvsoc *gvsoc_new();

}
