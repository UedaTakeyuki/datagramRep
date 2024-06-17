#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QDebug>
#include <QString>
#include <sys/select.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>
#include <errno.h>

#define BUF_SIZE 10             /* Maximum size of messages exchanged
                                   between client and server */

#define SV_SOCK_PATH "/tmp/ud_ucase"

class WorkerThread : public QThread {
  Q_OBJECT
  public:
    void run() {
      struct sockaddr_un svaddr, claddr;
      int sfd, j;
      fd_set rfds;
      ssize_t numBytes;
      socklen_t len;
      char buf[BUF_SIZE];

      // Create Unixdomain Datagram Socket
      sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
      if (sfd == -1)
          qDebug() << "socket";
      FD_ZERO(&rfds);
      FD_SET(sfd, &rfds);

      // Construct well-known address and bind server socket to it
      if (strlen(SV_SOCK_PATH) > sizeof(svaddr.sun_path) - 1)
          printf("Server socket path too long: %s", SV_SOCK_PATH);

      if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
          printf("remove-%s", SV_SOCK_PATH);

      memset(&svaddr, 0, sizeof(struct sockaddr_un));
      svaddr.sun_family = AF_UNIX;
      strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

      if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1)
          printf("bind");

      // Receive messages, convert to uppercase, and return to client
      while(1) {
          len = sizeof(struct sockaddr_un);

          // Before wait by select
          // emit progressChanged("before select");
//          qDebug() << "before select";
//          select(sfd + 1, &rfds, NULL, NULL, NULL);
//          qDebug() << "after select";
          qDebug() << "before recvfrom";
          emit progressChanged("before recvfrom");
          numBytes = recvfrom(sfd, buf, BUF_SIZE, 0,
                              (struct sockaddr *) &claddr, &len);
          qDebug() << "numBytes:" << numBytes;

          if (numBytes == -1)
              printf("recvfrom");

          printf("Server received %ld bytes from %s\n", (long) numBytes,
                  claddr.sun_path);

          buf[numBytes] = 0;
          qDebug() << "buf:" << buf;
          emit progressChanged(buf);
          qDebug() << "Emit finished";

          // Convert received messages to uppercase
          for (j = 0; j < numBytes; j++)
              buf[j] = toupper((unsigned char) buf[j]);

          // Return back it to client socket
          if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) !=
                  numBytes)
              printf("sendto");
      }
/*
      while(1) {
            // ... hard work
            // Now want to notify main thread:
            emit progressChanged("Some info");
      }
*/
    }
  // Define signal:
  signals:
    void progressChanged(QString info);
};
#endif // THREAD_H
