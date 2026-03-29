// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

MJSON_ENUM_BEGIN(VehicleType, "Vehicle Type", nullptr)
    MJSON_ENUM_UNIT(E_VEHICLE_CAR,        car)
    MJSON_ENUM_UNIT(E_VEHICLE_MOTORCYCLE, motorcycle)
    MJSON_ENUM_UNIT(E_VEHICLE_TRUCK,      truck)
MJSON_ENUM_END(VehicleType)

#define MJSON_BASE_OBJECT_NAME Vehicle
MJSON_POLYMORPHIC_OBJECT_BEGIN(
        "Vehicle",
        "Person's Vehicle")
    MJSON_BASE_OBJECT_BEGIN(VehicleType)
      MJSON_BASE_OBJECT_FIELD(int, id,
        "ID",
        "Unique identifier of the vehicle")
      MJSON_BASE_OBJECT_FIELD(std::string, model,
        "Model",
        "Vehicle model name")
    MJSON_BASE_OBJECT_END()
    MJSON_DERIVED_OBJECT_BEGIN(Car, E_VEHICLE_CAR)
      MJSON_DERIVED_OBJECT_FIELD(int, doors,
        "Doors",
        "Number of doors")
      MJSON_DERIVED_OBJECT_FIELD(std::string, fuel_type,
        "Fuel Type",
        "Type of fuel used by the vehicle")
    MJSON_DERIVED_OBJECT_END(Car)
    MJSON_DERIVED_OBJECT_BEGIN(Motorcycle, E_VEHICLE_MOTORCYCLE)
      MJSON_DERIVED_OBJECT_FIELD(int, engine_size,
        "Engine Size",
        "Size of the motorcycle's engine")
    MJSON_DERIVED_OBJECT_END(Motorcycle)
    MJSON_DERIVED_OBJECT_BEGIN(Truck, E_VEHICLE_TRUCK)
      MJSON_DERIVED_OBJECT_FIELD(int, payload_capacity,
        "Payload Capacity",
        "Maximum weight the truck can carry")
      MJSON_DERIVED_OBJECT_FIELD(std::string, cab_type,
        "Cab Type",
        "Type of cab for the truck")
    MJSON_DERIVED_OBJECT_END(Truck)
MJSON_POLYMORPHIC_OBJECT_END()
#undef MJSON_BASE_OBJECT_NAME

MJSON_OBJECT_BEGIN(Products, nullptr, nullptr)
    MJSON_FIELD(std::vector<std::unique_ptr<Vehicle>>, products,
        "Products",
        "List of vehicles")
MJSON_OBJECT_END(Products)
