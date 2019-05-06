#include "backgroundsubtraction.h"

//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

BackgroundSubtraction::BackgroundSubtraction()
{
    widget = new QLabel("Background subtraction");
}

QString BackgroundSubtraction::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    LOG_CAMPBELL() << "testing";

    switch (portType)
    {
    case PortType::In:
        return QStringLiteral("Input");
        break;
    case PortType::Out:
        if (portIndex == 0)
        {
            return QStringLiteral("orignal");
        }
        else if (portIndex == 1)
        {
            return QStringLiteral("threshold");
        }
        else if (portIndex == 2)
        {
            return QStringLiteral("results");
        }
        break;
    }
    return QString();
}

unsigned int BackgroundSubtraction::nPorts(PortType portType) const
{
    if (PortType::In == portType)
    {
        return 1;
    }
    else if (PortType::Out == portType)
    {
        return 3;
    }
    return 0;
}

NodeDataType BackgroundSubtraction::dataType(PortType, PortIndex) const
{
    return VideoGraphData().type();
}


std::shared_ptr<NodeData> BackgroundSubtraction::outData(PortIndex port)
{
    switch (port)
    {
    case 0:
        if (inData)
            return inData;
        break;
    case 1:
        if (threshold)
            return threshold;
        break;
    case 2:
        if (output)
            return output;
        break;
    }

    return NULL;
}

void BackgroundSubtraction::setInData(std::shared_ptr<NodeData> data, int location)
{
    cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorKNN();
    inData = std::dynamic_pointer_cast<VideoGraphData>(data);

    if (inData)
    {

        std::vector<cv::Mat> thresh;
        std::vector<cv::Mat> out;

        auto frames = inData->data();

        cv::Mat orig;
        cv::Mat final;
        cv::Mat mask;

        int  counter = 1;
        for (int it = 0; it < frames.size(); it++)
        {
            orig = frames[it];
            pMOG2->apply(orig, final);
            pMOG2->getBackgroundImage(mask);

            thresh[counter] = final.clone();
            out[counter] = mask.clone();
            counter++;
        }

        threshold = std::make_shared<VideoGraphData>(thresh);
        threshold->setFrameRate(inData->getFrameRate());
        output = std::make_shared<VideoGraphData>(out);
        output->setFrameRate(inData->getFrameRate());

        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();

    }
    else
    {
        threshold.reset();
        inData.reset();
        output.reset();

        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
    }
}

NodeValidationState BackgroundSubtraction::validationState() const
{
    return modelValidationState;
}

QString BackgroundSubtraction::validationMessage() const
{
    return modelValidationError;
}
