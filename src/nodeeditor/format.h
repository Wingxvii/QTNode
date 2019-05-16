#ifndef FORMAT_H
#define FORMAT_H

//Core
#include <QtCore/QObject>                                   //required include
#include <nodes/NodeDataModel>                              //required include
#include "analyzer/graphdataconnector.h"                    //required include

//Data types
#include "DataTypes/*.h"                                    //include all used data types

//Widgets
#include <QLabel>                                           //include QT widgets
#include <QGridLayout>                                      //atleast 1 layout


using QtNodes::PortType;                                    //Node Editor Functions
using QtNodes::PortIndex;                                   //Node Editor Functions
using QtNodes::NodeData;                                    //Node Editor Functions
using QtNodes::NodeDataType;                                //Node Editor Functions
using QtNodes::NodeDataModel;                               //Node Editor Functions
using QtNodes::NodeValidationState;                         //Node Editor Functions

class Format : public NodeDataModel{                        //class inheriates from NodeDataModel
    Q_OBJECT

}


#endif // FORMAT_H
