#include "blobfilternode.h"

BlobFilterNode::BlobFilterNode()
{
    button = new QPushButton("display filter");
}


QString BlobFilterNode::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    LOG_CAMPBELL() << "testing";
    return QString("Video");
}

unsigned int BlobFilterNode::nPorts(PortType portType) const
{
    if (PortType::In == portType)
    {
        return 1;
    }
    else if (PortType::Out == portType)
    {
        return 1;
    }
    return 0;
}

NodeDataType BlobFilterNode::dataType(PortType, PortIndex) const
{
    return VideoGraphData().type();
}


std::shared_ptr<NodeData> BlobFilterNode::outData(PortIndex port)
{
    return _data;
}

void BlobFilterNode::setInData(std::shared_ptr<NodeData> data, int location)
{
    cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorKNN();
    auto inData = std::dynamic_pointer_cast<VideoGraphData>(data);


    if (inData)
    {
        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();

        auto frames = inData->data();
        std::vector<cv::Mat> setData;

        cv::SimpleBlobDetector::Params test;
        test.blobColor = 255;
        test.filterByColor = true;
        test.filterByArea = true;
        test.minArea = 10000;
        test.maxArea = 10000000;

        test.filterByCircularity = false;
        test.filterByConvexity = false;
        test.filterByInertia = false;
        cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(test);

        // Detect blobs.
        std::vector<cv::KeyPoint> keypoints;

        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
        cv::Mat im_with_keypoints;
        float fps = inData->getFrameRate();
        for (int it = 0; it < frames.size(); it++)
        {
            detector->detect(frames[it], keypoints);
            cv::drawKeypoints(frames[it], keypoints, im_with_keypoints, cv::Scalar(255,0,0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
            // Show blobs

            if (keypoints.size() > 0)
            {
                LOG_CAMPBELL() << "create keypoint at " + QString::number(it);
                setData[it] = im_with_keypoints.clone();
            }
            cv::imshow("keypoints", im_with_keypoints);
            cv::waitKey( 1000 / fps );

        }
        cv::destroyWindow("keypoints");

        _data = std::make_shared<VideoGraphData>(setData);
        _data->setFrameRate(inData->getFrameRate());
    }
    else
    {
        _data.reset();
        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
    }
}

NodeValidationState BlobFilterNode::validationState() const
{
    return modelValidationState;
}

QString BlobFilterNode::validationMessage() const
{
    return modelValidationError;
}
