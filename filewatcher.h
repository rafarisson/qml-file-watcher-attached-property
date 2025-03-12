#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QQmlEngine>

class QFileSystemWatcher;

class FileWatcher : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use as attached property.")
    QML_ATTACHED(FileWatcher)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
    Q_PROPERTY(QString lastModified READ lastModified NOTIFY changed FINAL)

public:
    explicit FileWatcher(QObject *parent = nullptr);
    virtual ~FileWatcher() = default;

    static FileWatcher *qmlAttachedProperties(QObject *object);

    QString path() const;
    void setPath(const QString &path);
    QString lastModified() const;

private:
    void addPath(const QString &path);
    void setupDeletedFileManager();

private slots:
    void onFileChanged(const QString &path);
    void onDirectoryChanged(const QString &path);

signals:
    void pathChanged();
    void changed();

private:
    QFileSystemWatcher *m_watcher;
    QMetaObject::Connection m_directoryChangedConnection;
    QString m_absolutePath;
    QString m_path;
};

#endif // FILEWATCHER_H
