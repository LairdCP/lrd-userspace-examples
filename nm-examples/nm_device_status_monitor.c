/*
 * This example monitors NM device state via D-Bus "StateChanged" signal on
 * "org.freedesktop.NetworkManager.Device" object.
 * It uses GDBus and the libnm headers.
 */
#include <stdio.h>
#include <string.h>
#include "libnm_wrapper.h"

static const char *device_state_int2str(int state)
{
	const char *state_list[] = {
		"NM_DEVICE_STATE_UNKNOWN",
		"NM_DEVICE_STATE_UNMANAGED",
		"NM_DEVICE_STATE_UNAVAILABLE",
		"NM_DEVICE_STATE_DISCONNECTED",
		"NM_DEVICE_STATE_PREPARE",
		"NM_DEVICE_STATE_CONFIG",
		"NM_DEVICE_STATE_NEED_AUTH",
		"NM_DEVICE_STATE_IP_CONFIG",
		"NM_DEVICE_STATE_IP_CHECK",
		"NM_DEVICE_STATE_SECONDARIES",
		"NM_DEVICE_STATE_ACTIVATED",
		"NM_DEVICE_STATE_DEACTIVATING",
		"NM_DEVICE_STATE_FAILED"
	};
	return state_list[state/10];
}

static const char *device_state_reason_int2str(int reason)
{
	const char *reason_list[] = {
		"NM_DEVICE_STATE_REASON_NONE",
		"NM_DEVICE_STATE_REASON_UNKNOWN",
		"NM_DEVICE_STATE_REASON_NOW_MANAGED",
		"NM_DEVICE_STATE_REASON_NOW_UNMANAGED",
		"NM_DEVICE_STATE_REASON_CONFIG_FAILED",
		"NM_DEVICE_STATE_REASON_IP_CONFIG_UNAVAILABLE",
		"NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED",
		"NM_DEVICE_STATE_REASON_NO_SECRETS",
		"NM_DEVICE_STATE_REASON_SUPPLICANT_DISCONNECT",
		"NM_DEVICE_STATE_REASON_SUPPLICANT_CONFIG_FAILED",
		"NM_DEVICE_STATE_REASON_SUPPLICANT_FAILED",
		"NM_DEVICE_STATE_REASON_SUPPLICANT_TIMEOUT",
		"NM_DEVICE_STATE_REASON_PPP_START_FAILED",
		"NM_DEVICE_STATE_REASON_PPP_DISCONNECT",
		"NM_DEVICE_STATE_REASON_PPP_FAILED",
		"NM_DEVICE_STATE_REASON_DHCP_START_FAILED",
		"NM_DEVICE_STATE_REASON_DHCP_ERROR",
		"NM_DEVICE_STATE_REASON_DHCP_FAILED",
		"NM_DEVICE_STATE_REASON_SHARED_START_FAILED",
		"NM_DEVICE_STATE_REASON_SHARED_FAILED",
		"NM_DEVICE_STATE_REASON_AUTOIP_START_FAILED",
		"NM_DEVICE_STATE_REASON_AUTOIP_ERROR",
		"NM_DEVICE_STATE_REASON_AUTOIP_FAILED",
		"NM_DEVICE_STATE_REASON_MODEM_BUSY",
		"NM_DEVICE_STATE_REASON_MODEM_NO_DIAL_TONE",
		"NM_DEVICE_STATE_REASON_MODEM_NO_CARRIER",
		"NM_DEVICE_STATE_REASON_MODEM_DIAL_TIMEOUT",
		"NM_DEVICE_STATE_REASON_MODEM_DIAL_FAILED",
		"NM_DEVICE_STATE_REASON_MODEM_INIT_FAILED",
		"NM_DEVICE_STATE_REASON_GSM_APN_FAILED",
		"NM_DEVICE_STATE_REASON_GSM_REGISTRATION_NOT_SEARCHING",
		"NM_DEVICE_STATE_REASON_GSM_REGISTRATION_DENIED",
		"NM_DEVICE_STATE_REASON_GSM_REGISTRATION_TIMEOUT",
		"NM_DEVICE_STATE_REASON_GSM_REGISTRATION_FAILED",
		"NM_DEVICE_STATE_REASON_GSM_PIN_CHECK_FAILED",
		"NM_DEVICE_STATE_REASON_FIRMWARE_MISSING",
		"NM_DEVICE_STATE_REASON_REMOVED",
		"NM_DEVICE_STATE_REASON_SLEEPING",
		"NM_DEVICE_STATE_REASON_CONNECTION_REMOVED",
		"NM_DEVICE_STATE_REASON_USER_REQUESTED",
		"NM_DEVICE_STATE_REASON_CARRIER",
		"NM_DEVICE_STATE_REASON_CONNECTION_ASSUMED",
		"NM_DEVICE_STATE_REASON_SUPPLICANT_AVAILABLE",
		"NM_DEVICE_STATE_REASON_MODEM_NOT_FOUND",
		"NM_DEVICE_STATE_REASON_BT_FAILED",
		"NM_DEVICE_STATE_REASON_GSM_SIM_NOT_INSERTED",
		"NM_DEVICE_STATE_REASON_GSM_SIM_PIN_REQUIRED",
		"NM_DEVICE_STATE_REASON_GSM_SIM_PUK_REQUIRED",
		"NM_DEVICE_STATE_REASON_GSM_SIM_WRONG",
		"NM_DEVICE_STATE_REASON_INFINIBAND_MODE",
		"NM_DEVICE_STATE_REASON_DEPENDENCY_FAILED",
		"NM_DEVICE_STATE_REASON_BR2684_FAILED",
		"NM_DEVICE_STATE_REASON_MODEM_MANAGER_UNAVAILABLE",
		"NM_DEVICE_STATE_REASON_SSID_NOT_FOUND",
		"NM_DEVICE_STATE_REASON_SECONDARY_CONNECTION_FAILED",
		"NM_DEVICE_STATE_REASON_DCB_FCOE_FAILED",
		"NM_DEVICE_STATE_REASON_TEAMD_CONTROL_FAILED",
		"NM_DEVICE_STATE_REASON_MODEM_FAILED",
		"NM_DEVICE_STATE_REASON_MODEM_AVAILABLE",
		"NM_DEVICE_STATE_REASON_SIM_PIN_INCORRECT",
		"NM_DEVICE_STATE_REASON_NEW_ACTIVATION",
		"NM_DEVICE_STATE_REASON_PARENT_CHANGED",
		"NM_DEVICE_STATE_REASON_PARENT_MANAGED_CHANGED",
		"NM_DEVICE_STATE_REASON_OVSDB_FAILED",
		"NM_DEVICE_STATE_REASON_IP_ADDRESS_DUPLICATE",
		"NM_DEVICE_STATE_REASON_IP_METHOD_UNSUPPORTED",
		"NM_DEVICE_STATE_REASON_SRIOV_CONFIGURATION_FAILED",
		"NM_DEVICE_STATE_REASON_PEER_NOT_FOUND"
	};
	return reason_list[reason];
}

int cb_print_state_reason(int state, int reason)
{
	static int cnt = 10;
	printf("state %d (%s) reason %d (%s)\n", state, device_state_int2str(state), reason, device_state_reason_int2str(reason));
	return --cnt;
}

int main (int argc, char *argv[])
{
	char dev[32];
	libnm_wrapper_handle hd;
	LIBNM_WRAPPER_STATE_MONITOR_CALLBACK_ST cb;

	snprintf(dev, 32, "%s" , "wlan0");
	if(argc > 1)
		snprintf(dev, 32, "%s" , argv[1]);

	hd = libnm_wrapper_init();
	if(!hd) return -1;

	cb.callback = &cb_print_state_reason;

	libnm_wrapper_device_state_monitor(hd, dev, &cb);

	libnm_wrapper_destroy(hd);

	return 0;
}
