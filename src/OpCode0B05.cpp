/* Oracle Redo OpCode: 11.5
   Copyright (C) 2018-2020 Adam Leszczynski (aleszczynski@bersler.com)

This file is part of OpenLogReplicator.

OpenLogReplicator is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 3, or (at your option)
any later version.

OpenLogReplicator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenLogReplicator; see the file LICENSE;  If not see
<http://www.gnu.org/licenses/>.  */

#include "OpCode0B05.h"
#include "OracleAnalyzer.h"
#include "RedoLogRecord.h"

using namespace std;

namespace OpenLogReplicator {

    OpCode0B05::OpCode0B05(OracleAnalyzer *oracleAnalyzer, RedoLogRecord *redoLogRecord) :
            OpCode(oracleAnalyzer, redoLogRecord) {
    }

    OpCode0B05::~OpCode0B05() {
    }

    void OpCode0B05::process(void) {
        OpCode::process();
        uint64_t fieldNum = 0, fieldPos = 0;
        uint16_t fieldLength = 0;

        oracleAnalyzer->nextField(redoLogRecord, fieldNum, fieldPos, fieldLength);
        //field: 1
        ktbRedo(fieldPos, fieldLength);

        if (!oracleAnalyzer->nextFieldOpt(redoLogRecord, fieldNum, fieldPos, fieldLength))
            return;
        //field: 2
        kdoOpCode(fieldPos, fieldLength);
        redoLogRecord->nullsDelta = fieldPos + 26;
        uint8_t *nulls = redoLogRecord->data + redoLogRecord->nullsDelta;

        if (!oracleAnalyzer->nextFieldOpt(redoLogRecord, fieldNum, fieldPos, fieldLength))
            return;
        //field: 3
        redoLogRecord->colNumsDelta = fieldPos;
        uint8_t *colNums = redoLogRecord->data + redoLogRecord->colNumsDelta;

        if ((redoLogRecord->flags & FLAGS_KDO_KDOM2) != 0) {
            oracleAnalyzer->nextField(redoLogRecord, fieldNum, fieldPos, fieldLength);
            //field: 4
            redoLogRecord->rowData = fieldNum;
            if (oracleAnalyzer->dumpRedoLog >= 1)
                dumpColsVector(redoLogRecord->data + fieldPos, oracleAnalyzer->read16(colNums), fieldLength);
        } else {
            redoLogRecord->rowData = fieldNum + 1;
            uint8_t bits = 1;

            //fields: 4 + cc .. 4 + cc - 1
            for (uint64_t i = 0; i < redoLogRecord->cc; ++i) {
                oracleAnalyzer->nextField(redoLogRecord, fieldNum, fieldPos, fieldLength);

                if (oracleAnalyzer->dumpRedoLog >= 1)
                    dumpCols(redoLogRecord->data + fieldPos, oracleAnalyzer->read16(colNums), fieldLength, *nulls & bits);
                bits <<= 1;
                colNums += 2;
                if (bits == 0) {
                    bits = 1;
                    ++nulls;
                }
            }
        }
    }
}
