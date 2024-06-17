#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "WorkerThread.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [](){
        QTextStream qstdin(stdin);
        QString line = qstdin.readLine();
        if (!line.isEmpty()){
            qDebug() << line;
        }
    });

    // The issue is not reproduced when:
    // 1. Comment out the line below
    // 2. Or inclease interval as 60000
    timer->start(1000);
    //timer->start(60000);

    this->startWorkerThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onProgressChanged(QString info) {
    // Processing code
    qDebug() << "Receive Signal:" << info;
}

void MainWindow::startWorkerThread() {
    // Create an instance of your woker
    WorkerThread *workerThread = new WorkerThread;
    // Connect our signal and slot
    connect(workerThread, SIGNAL(progressChanged(QString)),
                          SLOT(onProgressChanged(QString)));
    // Setup callback for cleanup when it finishes
    connect(workerThread, SIGNAL(finished()),
            workerThread, SLOT(deleteLater()));
    // Run, Forest, run!
    workerThread->start(); // This invokes WorkerThread::run in a new thread
}
