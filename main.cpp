#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"  // JSON parsing library

using json = nlohmann::json;

// Define a struct to represent each root point (x, y)
struct Point {
    int x;
    long long y;
};

// Function for Lagrange interpolation to calculate the polynomial constant (c)
double lagrangeInterpolation(const std::vector<Point>& points, int x) {
    double result = 0.0;  // Final result (polynomial evaluated at x)
    int n = points.size();  // Number of points

    // Lagrange interpolation formula
    for (int i = 0; i < n; i++) {
        double term = points[i].y;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                term *= (x - points[j].x) / double(points[i].x - points[j].x);
            }
        }
        result += term;
    }
    return result;
}

int main() {
    // 1. Parse JSON Input
    std::ifstream file("input.json");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    json data;
    file >> data;

    // 2. Extract and decode the roots
    std::vector<Point> points;  // To store the decoded points (x, y)

    for (auto& element : data.items()) {
        if (element.key() != "keys") {  // Skip "keys"
            int base = std::stoi(element.value()["base"].get<std::string>());   // Base of the value
            std::string value_str = element.value()["value"].get<std::string>();  // Value string
            long long decoded_value = std::stoll(value_str, nullptr, base);  // Decode the value

            int x = std::stoi(element.key());  // Use the element key as the x-value
            points.push_back({x, decoded_value});  // Add (x, y) point to the vector
        }
    }

    // 3. Find the constant term (c) of the polynomial using Lagrange Interpolation
    double constant_term = lagrangeInterpolation(points, 0);  // Find c by evaluating at x = 0
    std::cout << "The constant term (c) of the polynomial is: " << constant_term << std::endl;

    return 0;
}
