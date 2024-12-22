#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize the central widget and layout
    QWidget *centralWidget = new QWidget(this); // Create the central widget
    setCentralWidget(centralWidget);           // Set it as the central widget of MainWindow

    videoWidget = new QWidget(this);           // Video playback widget
    layout = new QVBoxLayout();                // Layout
    layout->addWidget(videoWidget);            // Add the video widget to the layout

    // Add a button for opening video files
    QPushButton *openButton = new QPushButton("Open Video", this); // Create button
    layout->addWidget(openButton);                                // Add the button to the layout

    centralWidget->setLayout(layout);          // Apply the layout to the central widget

    // Connect the button's clicked signal to the slot
    connect(openButton, &QPushButton::clicked, this, &MainWindow::on_openButton_clicked);

    // Initialize libVLC
    const char *vlcArgs[] = { "--no-xlib" }; // Example: disable X11 if unnecessary
    vlcInstance = libvlc_new(1, vlcArgs);
    if (!vlcInstance) {
        std::cerr << "Failed to create VLC instance!" << std::endl;
    }
}

MainWindow::~MainWindow()
{
    // Cleanup libVLC resources
    if (mediaPlayer) {
        libvlc_media_player_stop(mediaPlayer);
        libvlc_media_player_release(mediaPlayer);
    }

    if (media) {
        libvlc_media_release(media);
    }

    if (vlcInstance) {
        libvlc_release(vlcInstance);
    }

    delete ui;
}

void MainWindow::on_openButton_clicked()
{
    // Open a file dialog to select a video file
    QString fileName = QFileDialog::getOpenFileName(this, "Open Video File", "", "Video Files (*.mp4 *.avi *.mkv *.flv)");
    if (!fileName.isEmpty()) {
        std::cout << "DEBUG FILE Found!" << std::endl;
        openVideo(fileName); // Call openVideo to open the video file
    }
}

void MainWindow::openVideo(const QString &filePath)
{
    if (!vlcInstance) {
        std::cerr << "VLC instance is not initialized!" << std::endl;
        return;
    }

    // Stop the current media if already playing
    if (mediaPlayer) {
        libvlc_media_player_stop(mediaPlayer);
        libvlc_media_player_release(mediaPlayer);
        mediaPlayer = nullptr;
    }

    if (media) {
        libvlc_media_release(media);
        media = nullptr;
    }

    // Convert file path to UTF-8
    QString normalizedPath = QDir::toNativeSeparators(filePath);
    QByteArray filePathUtf8 = normalizedPath.toUtf8();

    // Create a new libVLC media object
    media = libvlc_media_new_path(vlcInstance, filePathUtf8.constData());
    if (!media) {
        std::cerr << "Failed to create VLC media for file: " << filePath.toStdString() << std::endl;
        return;
    }

    // Create a new media player
    mediaPlayer = libvlc_media_player_new_from_media(media);
    if (!mediaPlayer) {
        std::cerr << "Failed to create VLC media player!" << std::endl;
        libvlc_media_release(media);
        media = nullptr;
        return;
    }

    // Set the video output widget for the media player
    libvlc_media_player_set_hwnd(mediaPlayer, reinterpret_cast<void *>(videoWidget->winId()));

    // Start playback
    if (libvlc_media_player_play(mediaPlayer) != 0) {
        std::cerr << "Failed to play the video!" << std::endl;
    } else {
        std::cout << "Playing video: " << filePath.toStdString() << std::endl;
    }
}
