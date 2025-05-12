from google.protobuf import empty_pb2 as _empty_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class Coordinate(_message.Message):
    __slots__ = ("coordX", "coordY")
    COORDX_FIELD_NUMBER: _ClassVar[int]
    COORDY_FIELD_NUMBER: _ClassVar[int]
    coordX: float
    coordY: float
    def __init__(self, coordX: _Optional[float] = ..., coordY: _Optional[float] = ...) -> None: ...

class OptimizationRequest(_message.Message):
    __slots__ = ("coordinates", "mutationRate", "individualNumber")
    COORDINATES_FIELD_NUMBER: _ClassVar[int]
    MUTATIONRATE_FIELD_NUMBER: _ClassVar[int]
    INDIVIDUALNUMBER_FIELD_NUMBER: _ClassVar[int]
    coordinates: _containers.RepeatedCompositeFieldContainer[Coordinate]
    mutationRate: float
    individualNumber: int
    def __init__(self, coordinates: _Optional[_Iterable[_Union[Coordinate, _Mapping]]] = ..., mutationRate: _Optional[float] = ..., individualNumber: _Optional[int] = ...) -> None: ...

class OptimizationReply(_message.Message):
    __slots__ = ("accepted", "coordinates")
    ACCEPTED_FIELD_NUMBER: _ClassVar[int]
    COORDINATES_FIELD_NUMBER: _ClassVar[int]
    accepted: bool
    coordinates: _containers.RepeatedCompositeFieldContainer[Coordinate]
    def __init__(self, accepted: bool = ..., coordinates: _Optional[_Iterable[_Union[Coordinate, _Mapping]]] = ...) -> None: ...

class UpdateNotification(_message.Message):
    __slots__ = ("coordinates",)
    COORDINATES_FIELD_NUMBER: _ClassVar[int]
    coordinates: _containers.RepeatedCompositeFieldContainer[Coordinate]
    def __init__(self, coordinates: _Optional[_Iterable[_Union[Coordinate, _Mapping]]] = ...) -> None: ...
