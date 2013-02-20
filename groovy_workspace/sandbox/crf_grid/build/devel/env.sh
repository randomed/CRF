#!/usr/bin/env sh
# generated from catkin/cmake/templates/env.sh.in

if [ $# -eq 0 ] ; then
  /bin/echo "Entering environment at '/home/random/CRF/groovy_workspace/sandbox/crf_grid/build/devel', type 'exit' to leave"
  . "/home/random/CRF/groovy_workspace/sandbox/crf_grid/build/devel/setup.sh"
  "$SHELL" -i
  /bin/echo "Exiting environment at '/home/random/CRF/groovy_workspace/sandbox/crf_grid/build/devel'"
else
  . "/home/random/CRF/groovy_workspace/sandbox/crf_grid/build/devel/setup.sh"
  exec "$@"
fi
