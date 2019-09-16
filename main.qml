import QtQml 2.13
import QtQuick 2.13
import QtQuick.Controls  2.13
import QtQuick.Window 2.13

Window {
  id: main_window
  flags: Qt.Window | Qt.Dialog | Qt.FramelessWindowHint
  visible: true
  color: "transparent"

  width: 0.8 * screen.width
  height: 0.7 * screen.height
  x: screen.virtualX + 0.5 * screen.width - 0.5 * width
  y: screen.virtualY + 0.1 * screen.height

  Rectangle {
    id: background
    color: "#AA444444"
    border.color: "#99CED6"
    border.width: 2
    width: 0
    height: 30

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
            duration: 400;
          }
          ScriptAction  { script: { header.state = "init"; } }
          NumberAnimation {
            property: "height"; easing.type: Easing.InOutQuad
            duration: 300;
          }
        }
      }
    ]

    Component.onCompleted: {
      onTriggered: background.state = "init"
    }
  }

  Rectangle {
    id: main_container
    anchors.margins: 30
    visible: true
    color: "transparent"
    width: main_window.width
    height: main_window.height

    Rectangle {
      id: header
      color: "#999C9EB0"
      radius: 5
      border.color: "#99CED6"
      border.width: 4
      anchors.top: parent.top
      anchors.left: parent.left
      height: 0
      width: parent.width

      states: [
        State {
          name: "init"
          PropertyChanges { target: header; height: 100 }
        }
      ]

      transitions: [
        Transition {
          to: "init"
          SequentialAnimation {
            NumberAnimation  {
              property: "height"; easing.type: Easing.InCubic
              duration: 60;
            }
            ScriptAction  { script: { main.state = "init"; } }
          }
        }
      ]
    }

    Rectangle {
      id: main
      color: "#CCFAF0AA"
      radius: 5
      anchors.top: header.bottom
      anchors.horizontalCenter: header.horizontalCenter
      height: 0
      width: 6 * header.width / 7

      states: [
        State {
          name: "init"
          PropertyChanges { target: main; height: parent.height - header.height }
        }
      ]

      transitions: [
        Transition {
          to: "init"
          SequentialAnimation {
            NumberAnimation  {
              property: "height"; easing.type: Easing.OutCubic
              duration: 250;
            }
          }
        }
      ]
    }
  }
}
