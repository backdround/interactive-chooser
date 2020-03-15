import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13

Window {
  id: main_window
  //flags: Qt.FramelessWindowHint | Qt.X11BypassWindowManagerHint | Qt.WindowTransparentForInput
  flags: Qt.FramelessWindowHint | Qt.X11BypassWindowManagerHint
  color: "transparent"

  ListView {
    anchors.fill: parent

    model: _model
    delegate: Pane {
      width: parent.width
      height: 85
      Label {
        anchors.fill: parent
        text: name
      }
      MouseArea {
        anchors.fill: parent
        onClicked: _model.action(index)
      }
    }
  }
}
