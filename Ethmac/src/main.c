/*
 * Copyright (c) 2023 Craig Peacock.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

void print_macaddr(void)
{
	struct net_if *iface = net_if_get_default();
	if (iface == NULL) {
		LOG_ERR("No default network interface found");
		return;
	} else {
		LOG_INF("Default network interface found");
	}

	struct net_linkaddr *linkaddr = net_if_get_link_addr(iface);
	if (linkaddr->type == NET_LINK_ETHERNET)
	{
		if (linkaddr->len == 6) {
			LOG_INF("Ethernet MAC Address %02X:%02X:%02X:%02X:%02X:%02X",
					linkaddr->addr[0], linkaddr->addr[1], linkaddr->addr[2],
					linkaddr->addr[3], linkaddr->addr[4], linkaddr->addr[5]);
		}
	}
}

int main(void)
{
	LOG_INF("WiFi/Ethernet Example");
	LOG_INF("Board: %s", CONFIG_BOARD);

	print_macaddr();

	return(0);
}
