import QtQuick 2.13
import QtQuick.Layouts 1.13

Rectangle {
  id: root
  x: 0
  y: parent.height * 0.833

  height: unit_k * 0.02
  width: target_width

  color: Style.unfocused_underline

  property bool activated: false
  property var target_width: parent.width
  property var active_color: "#fff"


  Rectangle {
    id: underline
    x: root.target_width * 0.5
    y: 0

    height: unit_k * 0.02
    width: 0

    color: root.active_color


    states: [
      State {
        name: "active"
        when: root.activated
        PropertyChanges {
          target: underline
          width: root.target_width
          x: 0
        }
      }
    ]

    transitions: [
      Transition {
        to: "active"
        ParallelAnimation {
          PropertyAnimation {
            duration: 300
            property: "x"
            easing.type: Easing.OutQuart
          }
          PropertyAnimation {
            duration: 300
            property: "width"
            easing.type: Easing.OutQuart
          }
        }
      }
    ]

  }
}
