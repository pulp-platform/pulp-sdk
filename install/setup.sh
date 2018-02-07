#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

gen_sh() {
	`cat $DIR/$1  | sed "s#PULP_SDK_HOME=.*#PULP_SDK_HOME=${DIR}#g" > $DIR/$1.new`
	mv $DIR/$1.new $DIR/$1
}

sh_scripts=`ls $DIR| grep .*.sh | grep -v setup | grep -v sourceme | grep -v .new`
csh_scripts=`ls $DIR| grep .*.csh | grep -v setup | grep -v sourceme | grep -v .new`

for script in $sh_scripts; do
	gen_sh $script
done
