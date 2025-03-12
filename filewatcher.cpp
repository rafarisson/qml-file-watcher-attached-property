#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QQuickTextDocument>

#include "filewatcher.h"

FileWatcher::FileWatcher(QObject *parent)
    : QObject{parent}
{
    m_watcher = new QFileSystemWatcher(this);
    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::onFileChanged);
}

FileWatcher *FileWatcher::qmlAttachedProperties(QObject *object)
{
    return new FileWatcher(object);
}

QString FileWatcher::path() const
{
    return m_path;
}

void FileWatcher::setPath(const QString &path)
{
    const QString urlPath = QUrl(path).toLocalFile();
    const QString filePath = urlPath.isEmpty() ? path : urlPath;
    const QString absolutePath = QFileInfo(filePath).absolutePath();

    if (m_path == filePath && m_absolutePath == absolutePath)
        return;

    if (m_watcher->files().length())
        m_watcher->removePaths(m_watcher->files());
    if (m_watcher->directories().length())
        m_watcher->removePaths(m_watcher->directories());

    m_path = filePath;
    m_absolutePath = absolutePath;

    addPath(m_path);
    addPath(m_absolutePath);
    setupDeletedFileManager();

    emit pathChanged();
    emit changed();
}

QString FileWatcher::lastModified() const
{
    return QFileInfo(m_path).lastModified().toString(QLocale::system().dateTimeFormat());
}

void FileWatcher::addPath(const QString &path)
{
    if (!path.isEmpty())
        m_watcher->addPath(path);
}

void FileWatcher::setupDeletedFileManager()
{
    if (QFileInfo::exists(m_path)) {
        if (m_directoryChangedConnection)
            disconnect(m_directoryChangedConnection);
    }
    else {
        if (!m_directoryChangedConnection)
            m_directoryChangedConnection = connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::onDirectoryChanged);
    }
}

void FileWatcher::onFileChanged(const QString &path)
{
    Q_UNUSED(path)

    setupDeletedFileManager();
    emit changed();
}

void FileWatcher::onDirectoryChanged(const QString &path)
{
    Q_UNUSED(path)

    if (!QFileInfo::exists(m_path))
        return;

    addPath(m_path);
    setupDeletedFileManager();
    emit changed();
}
