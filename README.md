# datagramRep
The reproducing environment of SO question of [The signal emmited from UnixDomain datagram socket server running on a QThread never received](https://stackoverflow.com/questions/78607353/the-signal-emmited-from-unixdomain-datagram-socket-server-running-on-a-qthread-n).

## Folder
- QT: Qt application
- testClient: UnixDomain Datagram client to send message to QT application for test.

## The most suspicious stuff to block to receive a signal.
Most suspicious stuff is ***[QTimer](https://github.com/UedaTakeyuki/datagramRep/blob/main/QT/reproduce/mainwindow.cpp#L11)*** object in ```mainwindow.cpp``` because issue ***doesn't reproduce*** when:

- QTimer doesn't start.
- The interval of QTimer increases to ***1 minute*** from 1 sec.
  The above threshold might be ***device dependent***. I've confirmed this with my ***RPi 2 Model B***.
