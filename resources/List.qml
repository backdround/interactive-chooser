import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

ListView {
  id: root

  model: _model

  interactive: false
  spacing: unit_k * 0.065

  property font font


  delegate: Rectangle {
    id: delegate
    width: parent.width
    height: unit_k * 0.45

    color: Style.list_element_background

    Label {
      anchors.centerIn: parent
      color: delegate.ListView.isCurrentItem ? Style.focused_text : Style.unfocused_text
      font: root.font

      text: name
    }

    states: [
      State {
        name: "selected"
        when: delegate.ListView.isCurrentItem
        PropertyChanges {
          target: delegate
          height: unit_k * 0.53
        }
      }
    ]

    transitions: [
      Transition {
        to: "selected"
        reversible: true
        PropertyAnimation {
          duration: 25
          property: "height"
          easing.type: Easing.OutInCirc
        }
      }
    ]

    MouseArea {
      anchors.fill: parent
      onClicked: _model.action(index)
      hoverEnabled: true
      onEntered: delegate.ListView.view.currentIndex = model.index
    }
  }

  highlight: Rectangle {
    z: 2
    color: "#30544575"
  }
  highlightMoveDuration: 25
}
