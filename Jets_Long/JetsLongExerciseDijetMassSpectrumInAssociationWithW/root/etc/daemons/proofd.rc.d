#! /bin/sh
#
# proofd	Start/Stop the PROOF daemon
#
# chkconfig: 345 20 80
# description:	The proofd daemon starts the PROOF worker processes.
#
# processname: proofd
# pidfile: /var/run/proofd.pid
# config:

PROOFD=$ROOTSYS/bin/proofd

# Source function library.
. /etc/init.d/functions

# Get config.
. /etc/sysconfig/network

# Get proofd config
[ -f /etc/sysconfig/proofd ] && . /etc/sysconfig/proofd

# Check that networking is up.
if [ ${NETWORKING} = "no" ]
then
	exit 0
fi

[ -x $PROOFD ] || exit 0

RETVAL=0
prog="proofd"

start() {
        echo -n $"Starting $prog: "
        # when not built with --prefix, add as last argument the
        # path where ROOT is installed
        daemon $PROOFD $PROOFDOPTS
        RETVAL=$?
        echo
        [ $RETVAL -eq 0 ] && touch /var/lock/subsys/proofd
	return $RETVAL
}

stop() {
    	test ! -f /var/lock/subsys/proofd && return 0 || true
        echo -n $"Stopping $prog: "
        killproc proofd
        RETVAL=$?
        echo
        [ $RETVAL -eq 0 ] && rm -f /var/lock/subsys/proofd
	return $RETVAL
}

# See how we were called.
case "$1" in
  start)
	start
	;;
  stop)
	stop
	;;
  status)
	status proofd
	;;
  restart|reload)
	stop
	start
	;;
  condrestart)
	if [ -f /var/lock/subsys/proofd ]; then
            stop
            start
        fi
	;;
  *)
	echo  $"Usage: $0 {start|stop|status|restart|reload|condrestart}"
	exit 1
esac

exit $RETVAL
