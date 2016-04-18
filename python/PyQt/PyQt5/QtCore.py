from PyQt5.QtCore import *

NULL = QVariant(QVariant.Int)

NULL.__nonzero__ = lambda: False
NULL.__repr__ = lambda: 'NULL'
NULL.__eq__ = lambda other: other.isNull() or other is None
NULL.__ne__ = lambda other: other.isNull() and other is not None
NULL.__hash__ = 2178309

QVariant.__hash__ = lambda x: hash(x.value())
