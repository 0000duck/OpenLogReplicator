/* Header for Schema class
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

#include <unordered_map>
#include <vector>

#include "types.h"

#ifndef SCHEMA_H_
#define SCHEMA_H_

using namespace std;

namespace OpenLogReplicator {
    class OracleAnalyzer;
    class OracleObject;
    class SchemaElement;

    struct SysObj {
        uint32_t objn;
        uint32_t objd;
        uint32_t owner;
        string name;
        uint64_t flags;
    };

    struct SysUser {
        uint64_t user;
        string name;
        uint64_t spare1;
    };

    class Schema {
    protected:
        stringstream& writeEscapeValue(stringstream &ss, string &str);
        unordered_map<typeobj, OracleObject*> objectMap;
        unordered_map<typeobj, OracleObject*> partitionMap;

    public:
        OracleObject *object;
        vector<SchemaElement*> elements;
        unordered_map<typeuser, SysUser*> sysUserMap;
        unordered_map<typeobj, SysObj*> sysObjMap;

        Schema();
        virtual ~Schema();

        bool readSchema(OracleAnalyzer *oracleAnalyzer);
        void writeSchema(OracleAnalyzer *oracleAnalyzer);
        OracleObject *checkDict(typeobj objn, typeobj objd);
        void addToDict(OracleObject *object);
        SchemaElement* addElement(void);
    };
}

#endif
