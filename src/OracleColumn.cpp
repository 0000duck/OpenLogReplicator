/* Column of a table in an Oracle database
   Copyright (C) 2018-2020 Adam Leszczynski.

This file is part of Open Log Replicator.

Open Log Replicator is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 3, or (at your option)
any later version.

Open Log Replicator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License
along with Open Log Replicator; see the file LICENSE.txt  If not see
<http://www.gnu.org/licenses/>.  */

#include <iostream>

#include "OracleColumn.h"

namespace OpenLogReplicator {

    OracleColumn::OracleColumn(uint64_t colNo, uint64_t segColNo, const string columnName, uint64_t typeNo, uint64_t length, int64_t precision,
            int64_t scale, uint64_t numPk, bool nullable) :
            colNo(colNo),
            segColNo(segColNo),
            columnName(columnName),
            typeNo(typeNo),
            length(length),
            precision(precision),
            scale(scale),
            numPk(numPk),
            nullable(nullable) {
    }

    OracleColumn::~OracleColumn() {
    }

    ostream& operator<<(ostream& os, const OracleColumn& column) {
        os << column.segColNo << ": (" << column.colNo << ", \"" << column.columnName << "\", " << column.typeNo << ", " << column.length << ")";
        return os;
    }
}
