//
//  FileLogger.h
//  interface/src
//
//  Created by Stojce Slavkovski on 12/22/13.
//  Copyright 2013 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_FileLogger_h
#define hifi_FileLogger_h

#include "AbstractLoggerInterface.h"
#include <QMutex>

class FileLogger : public AbstractLoggerInterface {
    Q_OBJECT

public:
    FileLogger(QObject* parent = NULL);

    virtual void addMessage(QString);
    virtual QStringList getLogData() { return _logData; };
    virtual void locateLog();

private:
    QStringList _logData;
    QString _fileName;
    QMutex _mutex;

};

#endif // hifi_FileLogger_h
