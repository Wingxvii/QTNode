#ifndef DECIMALDATA_H
#define DECIMALDATA_H


#include <nodes/NodeDataModel>
using QtNodes::NodeDataType;
using QtNodes::NodeData;


class DecimalData : public NodeData
{
public:
    DecimalData() : _number(0.0){}
    DecimalData(double const number) : _number(number) {}

    NodeDataType type() const override
    {
        return NodeDataType {"decimal", "Decimal"};
    }
    double number() const {return _number;}
    QString numberAsText() const {return QString::number(_number, 'f');}

private:
    double _number;
};

#endif // DECIMALDATA_H
