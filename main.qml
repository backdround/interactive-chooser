import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13

Window {
  id: main_window
  //flags: Qt.FramelessWindowHint | Qt.X11BypassWindowManagerHint | Qt.WindowTransparentForInput
  flags: Qt.FramelessWindowHint | Qt.Dialog
  color: "transparent"

  onVisibleChanged: {
    if (visible) {
      background.state = "init"
    }
  }

  Rectangle {
    id: background
    objectName: "background"
    color: "#AA444444"
    border.color: "#99CED6"
    border.width: 2
    width: 0
    height: 20

    states: [
      State {
        name: "init"
        PropertyChanges { target: background; width: main_window.width }
        PropertyChanges { target: background; height: main_window.height }
      }
    ]
    transitions: [
      Transition {
        to: "init"
        SequentialAnimation {
          //NumberAnimation {
          SmoothedAnimation {
            property: "width"; easing.type: Easing.InOutQuad
            duration: 300;
          }
          NumberAnimation {
            property: "height"; easing.type: Easing.InOutQuad
            duration: 250;
          }
        }
      }
    ]

    ColumnLayout {
      anchors.fill: parent
      anchors.margins: 40
      spacing: 20

      Rectangle {
        id: header
        color: "#999C9EB0"
        border.color: "#99FEF6"
        border.width: 4
        Layout.fillWidth: true
        Layout.minimumHeight: parent.height < 70 ? 0 : 70
        Layout.preferredHeight: parent.height < 70 ? parent.height : parent.height / 20
      }

      Rectangle {
        id: main
        color: "#CCFAF0AA"
        radius: 5
        Layout.fillWidth: true
        Layout.fillHeight: true
      }
    }
  }
}
