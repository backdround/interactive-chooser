import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

ListView {
  id: root

  model: _model

  interactive: false
  spacing: relative_unit * 0.065

  property font font

  property int current_id_
  function action(id) {
    if (id !== undefined) {
      _model.action(id)
    } else if (current_id_ !== undefined) {
      _model.action(current_id_)
    }
  }

  function reset_current() {
    currentIndex = 0
  }


  delegate: Rectangle {
    id: delegate
    width: parent.width
    height: relative_unit * 0.45

    color: Style.list_element_background

    Label {
      anchors.centerIn: parent
      color: delegate.ListView.isCurrentItem ? Style.focused_text : Style.unfocused_text
      font: root.font

      text: name + " (" + weight + ")"
    }

    states: [
      State {
        name: "selected"
        when: delegate.ListView.isCurrentItem
        PropertyChanges {
          target: delegate
          height: relative_unit * 0.53
        }
      }
    ]

    ListView.onIsCurrentItemChanged: {
      if (ListView.isCurrentItem) {
        root.current_id_ = model.id
      }
    }
  }

  highlight: Rectangle {
    z: 2
    color: "#30544575"
  }
  highlightMoveDuration: 25
}
