#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <format>
#include <string_view>
#include <iostream>
using namespace std::literals;

int main()
{
	constexpr auto requestTemplate     = "https://telelink.city/api/v1/949021bc-c2c0-43ad-a146-20e19bbc3649/transport/planner/stops/{}/times"sv;
	constexpr auto stopsApiUrl         = "https://telelink.city/api/v1/949021bc-c2c0-43ad-a146-20e19bbc3649/transport/planner/stops"sv;
	constexpr auto routesApiUrl        = "https://telelink.city/api/v1/949021bc-c2c0-43ad-a146-20e19bbc3649/transport/planner/routes"sv;
	constexpr auto stopId              = 340; // TODO: Remove after implementing requesting

	// Initialize vector with all stop IDs
	std::vector<int> stopIds;
	for (const auto& stops = nlohmann::json::parse(Get(cpr::Url{stopsApiUrl}).text);
		 const auto& stop : stops)
		stopIds.push_back(stop.at("id").get<int>());
		
	std::string_view stopNames[380];
	for (const auto stops = nlohmann::json::parse(Get(cpr::Url{stopsApiUrl}).text);
		 const auto& stop : stops)
		stopNames[stop.at("id").get<int>()] = stop.at("name").get<std::string_view>();

	std::vector<int> routeIds;
	for (const auto& routes = nlohmann::json::parse(Get(cpr::Url{routesApiUrl}).text);
		 const auto& route : routes)
		routeIds.push_back(route.at("id").get<int>());

	std::string_view routeNames[70];
	for (const auto& routes = nlohmann::json::parse(Get(cpr::Url{routesApiUrl}).text);
		 const auto& route : routes)
		routeNames[route.at("id").get<int>()] = route.at("shortName").get<std::string_view>();

	const cpr::Response response = Get(cpr::Url{std::format(requestTemplate, stopId)});
	const nlohmann::json body = nlohmann::json::parse(response.text);

	if (response.status_code == 200)
		std::cout << std::setw(4) << body << '\n';
}