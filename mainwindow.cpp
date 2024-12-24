#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QTextEdit>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QRegularExpression>
#include <QTimer>
#include <QDebug>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize the central widget and layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    videoWidget = new QWidget(this);
    layout = new QVBoxLayout();
    layout->addWidget(videoWidget);

    // Create and add the subtitle QLabel widget
    subtitleLabel = new QLabel(this);
    subtitleLabel->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    subtitleLabel->setFont(QFont("Noto Sans CJK", 12));
    layout->addWidget(subtitleLabel);

    QPushButton *openButton = new QPushButton("Open Video", this);
    layout->addWidget(openButton);

    centralWidget->setLayout(layout);

    // Connect button to open file dialog
    connect(openButton, &QPushButton::clicked, this, &MainWindow::on_openButton_clicked);

    // Initialize libVLC
    const char *vlcArgs[] = { "--no-xlib" };
    vlcInstance = libvlc_new(1, vlcArgs);
    if (!vlcInstance) {
        std::cerr << "Failed to create VLC instance!" << std::endl;
    }
}

// Destructor
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

// Slot to update subtitle text
void MainWindow::updateSubtitle(const QString &subtitleText)
{
    // Update the QLabel to display the subtitle text
     qDebug() <<"DEBUG subtitleText "<<subtitleText;
    subtitleLabel->setText(subtitleText);
}

// Open video and handle subtitle file
void MainWindow::openVideo(const QString &filePath)
{
    if (!vlcInstance) {
        std::cerr << "VLC instance is not initialized!" << std::endl;
        return;
    }

    if (mediaPlayer) {
        libvlc_media_player_stop(mediaPlayer);
        libvlc_media_player_release(mediaPlayer);
        mediaPlayer = nullptr;
    }

    if (media) {
        libvlc_media_release(media);
        media = nullptr;
    }

    // Convert file path to UTF-8 format
    QString fileNormalizedPath = QDir::toNativeSeparators(filePath);
    QByteArray filePathUtf8 = fileNormalizedPath.toUtf8();
    QString subtitleNormalizedPath = QDir::toNativeSeparators("D:/Project/cppProject/ListenUp/testvideos/test1.srt");
    QByteArray subtitlePathUtf8 = subtitleNormalizedPath.toUtf8();

    // Create new libVLC media object
    media = libvlc_media_new_path(vlcInstance, filePathUtf8.constData());
    if (!media) {
        std::cerr << "Failed to create VLC media object for: " << filePath.toStdString() << std::endl;
        return;
    }

    // Create media player
    mediaPlayer = libvlc_media_player_new_from_media(media);
    if (!mediaPlayer) {
        std::cerr << "Failed to create VLC media player!" << std::endl;
        libvlc_media_release(media);
        media = nullptr;
        return;
    }

    // Set video output window
    libvlc_media_player_set_hwnd(mediaPlayer, reinterpret_cast<void *>(videoWidget->winId()));

    // Use Media Changed event as an alternative to subtitle event
    libvlc_event_manager_t *eventManager = libvlc_media_player_event_manager(mediaPlayer);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerMediaChanged,
        [](const libvlc_event_t *event, void *data) {
            MainWindow *window = static_cast<MainWindow*>(data);
            // Here you could check for any media state changes or periodically update subtitles
        }, this);

    // Start video playback
    if (libvlc_media_player_play(mediaPlayer) != 0) {
        std::cerr << "Failed to play the video!" << std::endl;
    } else {
        std::cout << "Playing video: " << filePath.toStdString() << std::endl;
    }

    // Set up a timer to periodically update subtitles
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this, subtitlePathUtf8]() {
        // Get the current playback time in seconds
        int currentTimeInMillis = libvlc_media_player_get_time(mediaPlayer);
        int currentTimeInSeconds = currentTimeInMillis / 1000;

        // Get subtitle text for the current time
        QString subtitle = getSubtitleAtTime(subtitlePathUtf8, currentTimeInSeconds);

        // Update the subtitle label with the new subtitle
        updateSubtitle(subtitle);
    });
    timer->start(100); // Update every 100 ms
}

// Function to parse the SRT file and return the subtitle text at the given timestamp
QString MainWindow::getSubtitleAtTime(const QString &srtFilePath, int currentTimeInSeconds)
{
    QFile file(srtFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString subtitleText = "";
    QString line;
    int startTime = 0;
    int endTime = 0;
    QRegularExpression timeRegex("(\\d{2}):(\\d{2}):(\\d{2}),(\\d{3}) --> (\\d{2}):(\\d{2}):(\\d{2}),(\\d{3})");

    while (!in.atEnd()) {
        line = in.readLine();
        QRegularExpressionMatch match = timeRegex.match(line);

        qDebug() << "DEBUG Line: " << line;

        // Match the subtitle time range
        if (match.hasMatch()) {
            // Convert start time to seconds
            startTime = match.captured(1).toInt() * 3600 + match.captured(2).toInt() * 60 + match.captured(3).toInt();
            // Convert end time to seconds
            endTime = match.captured(5).toInt() * 3600 + match.captured(6).toInt() * 60 + match.captured(7).toInt();

            // If the current time is within this subtitle's time range, start accumulating subtitle text
            if (currentTimeInSeconds >= startTime && currentTimeInSeconds <= endTime) {
                // Clear previous subtitle text
                subtitleText.clear();
                // Continue reading until an empty line (end of subtitle block)
                while (!in.atEnd()) {
                    line = in.readLine().trimmed();
                    if (line.isEmpty()) {
                        break; // Empty line indicates the end of the current subtitle block
                    }
                    subtitleText += line + "\n"; // Accumulate each subtitle line
                }
                subtitleText = subtitleText.trimmed(); // Remove the last newline character to keep the format clean
                break; // Exit after finding the subtitle corresponding to the current time
            }
        }
    }

    file.close();
    return subtitleText;
}
