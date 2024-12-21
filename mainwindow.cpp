#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Video File", "", "Video Files (*.mp4 *.avi *.mkv *.flv)");
    if (!fileName.isEmpty()) {
        std::cout << "DEBUG FILE Found!" << std::endl;
        openVideo(fileName);  // Call openVideo to open the video file
    }
}

void MainWindow::openVideo(const QString &filePath)
{
    AVFormatContext *formatContext = nullptr;

    // Open the video file
    if (avformat_open_input(&formatContext, filePath.toStdString().c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open the file." << std::endl;
        return;
    }

    // Find stream information
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cerr << "Could not find stream information." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    // Find the video stream
    int videoStreamIndex = -1;
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        std::cerr << "No video stream found." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    // Get the codec context for the video stream
    AVCodecContext *codecContext = avcodec_alloc_context3(nullptr);
    if (!codecContext) {
        std::cerr << "Failed to allocate codec context." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    if (avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar) < 0) {
        std::cerr << "Failed to copy codec parameters to codec context." << std::endl;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    // Find the decoder
    const AVCodec* codec = avcodec_find_decoder(codecContext->codec_id);  // Corrected to const AVCodec*
    if (!codec) {
        std::cerr << "Codec not found." << std::endl;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    // Open the codec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cerr << "Failed to open codec." << std::endl;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    // Read video frames and decode them
    AVPacket packet;
    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        std::cerr << "Could not allocate frame." << std::endl;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    // Start decoding
    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecContext, &packet) < 0) {
                std::cerr << "Error sending packet for decoding." << std::endl;
                break;
            }

            while (avcodec_receive_frame(codecContext, frame) >= 0) {
                // Here you process the decoded frame, e.g., display it on the screen
                std::cout << "Decoded a frame." << std::endl;
                renderFrame(frame); // Call the renderFrame function to display the frame
            }
        }

        av_packet_unref(&packet);
    }

    // Cleanup
    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}

void MainWindow::renderFrame(AVFrame *frame)
{
    // Get the width and height of the video frame
    int width = frame->width;
    int height = frame->height;

    // Convert the frame from YUV format to RGB format
    SwsContext *swsContext = sws_getContext(width, height, static_cast<AVPixelFormat>(frame->format),
                                             width, height, AV_PIX_FMT_RGB24,
                                             SWS_BILINEAR, nullptr, nullptr, nullptr);  // Fixed the pixel format

    if (!swsContext) {
        std::cerr << "Failed to create SwsContext." << std::endl;
        return;
    }

    uint8_t *rgbData = nullptr;
    int rgbStride = 0;
    av_image_alloc(&rgbData, &rgbStride, width, height, AV_PIX_FMT_RGB24, 1);

    sws_scale(swsContext, frame->data, frame->linesize, 0, height, &rgbData, &rgbStride);

    // Create a QImage from the RGB data
    QImage img(rgbData, width, height, rgbStride, QImage::Format_RGB888);

    // Render the frame to the Qt window
    QPainter painter(this);
    painter.drawImage(0, 0, img);

    // Free the allocated memory
    av_freep(&rgbData);
    sws_freeContext(swsContext);
}
