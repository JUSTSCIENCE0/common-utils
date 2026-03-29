// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "example.hpp"

#include <iostream>

int main() {
    Products example{};
    std::unique_ptr<Vehicle> car_example = std::make_unique<Car>();
    std::unique_ptr<Vehicle> motorcycle_example = std::make_unique<Motorcycle>();
    std::unique_ptr<Vehicle> truck_example = std::make_unique<Truck>();

    // Set properties for the car
    auto car_ptr = static_cast<Car*>(car_example.get());
    car_ptr->id = 1;
    car_ptr->model = "Toyota Camry";
    car_ptr->doors = 4;
    car_ptr->fuel_type = "Gasoline";

    // Set properties for the motorcycle
    auto motorcycle_ptr = static_cast<Motorcycle*>(motorcycle_example.get());
    motorcycle_ptr->id = 2;
    motorcycle_ptr->model = "Harley Davidson";
    motorcycle_ptr->engine_size = 1800;

    // Set properties for the truck
    auto truck_ptr = static_cast<Truck*>(truck_example.get());
    truck_ptr->id = 3;
    truck_ptr->model = "Ford F-1";
    truck_ptr->payload_capacity = 200;
    truck_ptr->cab_type = "Crew Cab";

    // Add vehicles to a collection
    example.products.push_back(std::move(car_example));
    example.products.push_back(std::move(motorcycle_example));
    example.products.push_back(std::move(truck_example));

    // Serialize the vehicles to JSON
    std::string json = "";
    macrojson::object_to_json_str(example, json);

    std::cout << "Serialized JSON:\n" << json << "\n";

    Products example_output{};
    std::string error_descr;
    auto err_code = macrojson::json_str_to_object(json, example_output, error_descr);
    if (err_code != macrojson::E_MJSON_OK) {
        std::cerr << "Failed to parse JSON: " << error_descr << std::endl;
        return -1;
    }

    std::cout << "\nDeserialized objects:" << std::endl;
    for (const auto& vehicle: example_output.products) {
        std::cout << "Vehicle type: " << macrojson::to_string(vehicle->type) << std::endl;
    }

    std::cout << "\nString to enum example:" << std::endl;
    for (const auto& enum_unit : {
            E_VEHICLE_CAR,
            E_VEHICLE_MOTORCYCLE,
            E_VEHICLE_TRUCK
        }
    ) {
        std::cout << "Enum (" << enum_unit << ") - " << macrojson::to_string(enum_unit) << std::endl;
    }

    return 0;
}