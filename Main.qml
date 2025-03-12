import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Controls.Basic

import QtCore

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        onAccepted: filePath.text = Qt.resolvedUrl(selectedFile)
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: qsTr("File:")
            }
            TextField {
                id: filePath
                Layout.fillWidth: true
                FileWatcher.path: text
                FileWatcher.onChanged: fileInfo.text = FileWatcher.lastModified
            }
            Button {
                text: qsTr("...")
                onClicked: fileDialog.open()
            }
        }
        Label {
            id: fileInfo
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Select a file...")
        }
    }
}
