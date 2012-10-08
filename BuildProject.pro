

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += Api TonicTones OpenExrLoader LdrFormatsLoader ReinhardGlobalOperator ReinhardLocalOperator

Api.file = Src/Api/Api.pro

TonicTones.depends = Api
TonicTones.file = Src/TonicTones.pro

OpenExrLoader.depends = Api
OpenExrLoader.file = Src/Loaders/OpenExrLoader/OpenExrLoader.pro

LdrFormatsLoader.depends = Api
LdrFormatsLoader.file = Src/Loaders/LdrFormatsLoader/LdrFormatsLoader.pro

ReinhardGlobalOperator.depends = Api
ReinhardGlobalOperator.file = Src/Operators/ReinhardGlobalOperator/ReinhardGlobalOperator.pro

ReinhardLocalOperator.depends = Api
ReinhardLocalOperator.file = Src/Operators/ReinhardLocalOperator/ReinhardLocalOperator.pro


		   

