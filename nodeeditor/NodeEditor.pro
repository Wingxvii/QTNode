TEMPLATE = lib
CONFIG += staticlib
CONFIG += create_prl

DEFINES += NODE_EDITOR_SHARED
DEFINES += NODEEDITOR_LIBRARY

INCLUDEPATH += ./include
INCLUDEPATH += ./include/nodes/internal

HEADERS += \
    include/nodes/internal/Compiler.hpp \
    include/nodes/internal/Connection.hpp \
    include/nodes/internal/ConnectionGeometry.hpp \
    include/nodes/internal/ConnectionGraphicsObject.hpp \
    include/nodes/internal/ConnectionState.hpp \
    include/nodes/internal/ConnectionStyle.hpp \
    include/nodes/internal/DataModelRegistry.hpp \
    include/nodes/internal/Export.hpp \
    include/nodes/internal/FlowScene.hpp \
    include/nodes/internal/FlowView.hpp \
    include/nodes/internal/FlowViewStyle.hpp \
    include/nodes/internal/memory.hpp \
    include/nodes/internal/Node.hpp \
    include/nodes/internal/NodeData.hpp \
    include/nodes/internal/NodeDataModel.hpp \
    include/nodes/internal/NodeGeometry.hpp \
    include/nodes/internal/NodeGraphicsObject.hpp \
    include/nodes/internal/NodePainterDelegate.hpp \
    include/nodes/internal/NodeState.hpp \
    include/nodes/internal/NodeStyle.hpp \
    include/nodes/internal/OperatingSystem.hpp \
    include/nodes/internal/PortType.hpp \
    include/nodes/internal/QStringStdHash.hpp \
    include/nodes/internal/QUuidStdHash.hpp \
    include/nodes/internal/Serializable.hpp \
    include/nodes/internal/Style.hpp \
    include/nodes/internal/TypeConverter.hpp \
    include/nodes/Connection \
    include/nodes/ConnectionStyle \
    include/nodes/DataModelRegistry \
    include/nodes/FlowScene \
    include/nodes/FlowView \
    include/nodes/FlowViewStyle \
    include/nodes/Node \
    include/nodes/NodeData \
    include/nodes/NodeDataModel \
    include/nodes/NodeGeometry \
    include/nodes/NodePainterDelegate \
    include/nodes/NodeState \
    include/nodes/NodeStyle \
    include/nodes/TypeConverter \
    src/ConnectionBlurEffect.hpp \
    src/ConnectionPainter.hpp \
    src/NodeConnectionInteraction.hpp \
    src/NodePainter.hpp \
    src/Properties.hpp \
    src/StyleCollection.hpp \
    include/user/decimaldata.h \
    include/user/numbersourcedatamodel.h

SOURCES += \
    src/Connection.cpp \
    src/ConnectionBlurEffect.cpp \
    src/ConnectionGeometry.cpp \
    src/ConnectionGraphicsObject.cpp \
    src/ConnectionPainter.cpp \
    src/ConnectionState.cpp \
    src/ConnectionStyle.cpp \
    src/DataModelRegistry.cpp \
    src/FlowScene.cpp \
    src/FlowView.cpp \
    src/FlowViewStyle.cpp \
    src/Node.cpp \
    src/NodeConnectionInteraction.cpp \
    src/NodeDataModel.cpp \
    src/NodeGeometry.cpp \
    src/NodeGraphicsObject.cpp \
    src/NodePainter.cpp \
    src/NodeState.cpp \
    src/NodeStyle.cpp \
    src/Properties.cpp \
    src/StyleCollection.cpp \
    include/user/decimaldata.cpp \
    include/user/numbersourcedatamodel.cpp

