#include <catch2/catch.hpp>

#include "VkBootstrap.cpp"


TEST_CASE("Single Queue Device", "[UnitTests.queue_selection_logic]") {
	std::vector<VkQueueFamilyProperties> families = { VkQueueFamilyProperties{
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } } };

	REQUIRE(0 == vkb::detail::get_graphics_queue_index(families));
	REQUIRE(vkb::detail::QUEUE_INDEX_MAX_VALUE ==
	        vkb::detail::get_separate_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
	REQUIRE(vkb::detail::QUEUE_INDEX_MAX_VALUE ==
	        vkb::detail::get_separate_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
	REQUIRE(vkb::detail::QUEUE_INDEX_MAX_VALUE ==
	        vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
	REQUIRE(vkb::detail::QUEUE_INDEX_MAX_VALUE ==
	        vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
}

TEST_CASE("Dedicated Compute Queue, Separate Transfer", "[UnitTests.queue_selection_logic]") {
	SECTION("Dedicated Queue First") {
		std::vector<VkQueueFamilyProperties> families = {
			VkQueueFamilyProperties{
			    VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } },
			VkQueueFamilyProperties{ VK_QUEUE_COMPUTE_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } },
			VkQueueFamilyProperties{ VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } }
		};

		REQUIRE(0 == vkb::detail::get_graphics_queue_index(families));
		REQUIRE(1 == vkb::detail::get_separate_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
		REQUIRE(2 == vkb::detail::get_separate_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
		REQUIRE(1 == vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
		REQUIRE(vkb::detail::QUEUE_INDEX_MAX_VALUE ==
		        vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
	}
	SECTION("Dedicated Queue Last") {
		std::vector<VkQueueFamilyProperties> families = {
			VkQueueFamilyProperties{
			    VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } },
			VkQueueFamilyProperties{ VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } },
			VkQueueFamilyProperties{ VK_QUEUE_COMPUTE_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } }
		};

		REQUIRE(0 == vkb::detail::get_graphics_queue_index(families));
		REQUIRE(2 == vkb::detail::get_separate_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
		REQUIRE(1 == vkb::detail::get_separate_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
		REQUIRE(2 == vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
		REQUIRE(vkb::detail::QUEUE_INDEX_MAX_VALUE ==
		        vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
	}
}

TEST_CASE("Dedicated Transfer Queue, Separate Compute", "[UnitTests.queue_selection_logic]") {
	SECTION("Dedicated Queue First") {
		std::vector<VkQueueFamilyProperties> families = {
			VkQueueFamilyProperties{
			    VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } },
			VkQueueFamilyProperties{ VK_QUEUE_TRANSFER_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } },
			VkQueueFamilyProperties{ VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } }
		};

		REQUIRE(0 == vkb::detail::get_graphics_queue_index(families));
		REQUIRE(2 == vkb::detail::get_separate_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
		REQUIRE(1 == vkb::detail::get_separate_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
		REQUIRE(vkb::detail::QUEUE_INDEX_MAX_VALUE ==
		        vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
		REQUIRE(1 == vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
	}
	SECTION("Dedicated Queue Last") {
		std::vector<VkQueueFamilyProperties> families = {
			VkQueueFamilyProperties{
			    VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } },
			VkQueueFamilyProperties{ VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } },
			VkQueueFamilyProperties{ VK_QUEUE_TRANSFER_BIT, 1, 0, VkExtent3D{ 1, 1, 1 } }
		};

		REQUIRE(0 == vkb::detail::get_graphics_queue_index(families));
		REQUIRE(1 == vkb::detail::get_separate_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
		REQUIRE(2 == vkb::detail::get_separate_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
		REQUIRE(vkb::detail::QUEUE_INDEX_MAX_VALUE ==
		        vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT));
		REQUIRE(2 == vkb::detail::get_dedicated_queue_index(families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT));
	}
}

TEST_CASE("Device Queue querying", "[UnitTests.queue_selection_logic]") {
    Device device;
    device.queue_families.

}
TEST_CASE("Queue Selection logic", "[VkBootstrap.queue_logic]") {
	vkb::InstanceBuilder builder;

	auto instance_ret = builder.request_validation_layers().build();
	REQUIRE(instance_ret.has_value());

	vkb::destroy_instance(instance_ret.value());
}