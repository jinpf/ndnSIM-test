#!/bin/sh
# @Author: jinpf
# @Date:   2015-03-29 13:48:27
# @Last Modified by:   jinpf
# @Last Modified time: 2015-03-29 13:52:20

# this script is used for manual revert back to the origin code
cp -rfu ns-3/scratch/ ../ns-3/
cp -rfu ns-3/src/ndnSIM/apps ../ns-3/src/ndnSIM/
cp -rfu ns-3/src/ndnSIM/NFD/daemon ../ns-3/src/ndnSIM/NFD/
cp -rfu ns-3/src/ndnSIM/NFD/core ../ns-3/src/ndnSIM/NFD/
cp -rfu ns-3/src/ndnSIM/utils/tracers ../ns-3/src/ndnSIM/utils/
