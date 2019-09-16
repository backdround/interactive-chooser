import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13

Window {
  id: main_window
  flags: Qt.Window | Qt.Dialog | Qt.FramelessWindowHint
  visible: true
  color: "transparent"

  width: 0.42 * screen.width
  height: 0.85 * screen.height
  x: screen.virtualX + 0.5 * screen.width - 0.5 * width
  y: screen.virtualY + 0.5 * screen.height - 0.5 * height

  Rectangle {
    id: background
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
          NumberAnimation {
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

    Component.onCompleted: {
      onTriggered: background.state = "init"
    }

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
