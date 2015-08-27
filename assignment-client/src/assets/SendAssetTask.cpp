//
//  SendAssetTask.cpp
//
//  Created by Ryan Huffman on 2015/08/26
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "SendAssetTask.h"

#include <QFile>

#include <DependencyManager.h>
#include <NetworkLogging.h>
#include <NLPacket.h>
#include <NLPacketList.h>
#include <NodeList.h>

#include "AssetUtils.h"

SendAssetTask::SendAssetTask(MessageID messageID, const QByteArray& assetHash, QString filePath, DataOffset start, DataOffset end,
              const SharedNodePointer& sendToNode) :
    QRunnable(),
    _messageID(messageID),
    _assetHash(assetHash),
    _filePath(filePath),
    _start(start),
    _end(end),
    _sendToNode(sendToNode)
{
}

void SendAssetTask::run() {
    qDebug() << "Starting task to send asset: " << _assetHash << " for messageID " << _messageID;
    auto replyPacketList = std::unique_ptr<NLPacketList>(new NLPacketList(PacketType::AssetGetReply, QByteArray(), true, true));

    replyPacketList->write(_assetHash, HASH_HEX_LENGTH);

    replyPacketList->writePrimitive(_messageID);

    const int64_t MAX_LENGTH = 4294967296;

    if (_end <= _start) {
        writeError(replyPacketList.get(), AssetServerError::INVALID_BYTE_RANGE);
    } else if (_end - _start > MAX_LENGTH) {
        writeError(replyPacketList.get(), AssetServerError::INVALID_BYTE_RANGE);
    } else {
        QFile file { _filePath };

        if (file.open(QIODevice::ReadOnly)) {
            if (file.size() < _end) {
                writeError(replyPacketList.get(), AssetServerError::INVALID_BYTE_RANGE);
                qCDebug(networking) << "Bad byte range: " << _assetHash << " " << _start << ":" << _end;
            } else {
                auto size = _end - _start;
                file.seek(_start);
                replyPacketList->writePrimitive(AssetServerError::NO_ERROR);
                replyPacketList->writePrimitive(size);
                replyPacketList->write(file.read(size));
            }
            file.close();
            qCDebug(networking) << "Sending asset: " << _assetHash;
        } else {
            qCDebug(networking) << "Asset not found: " << _assetHash;
            writeError(replyPacketList.get(), AssetServerError::ASSET_NOT_FOUND);
        }
    }

    auto nodeList = DependencyManager::get<NodeList>();
    nodeList->sendPacketList(std::move(replyPacketList), *_sendToNode);
}
