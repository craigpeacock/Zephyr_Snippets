# USB-C PD (Power Delivery) Example

This example is pretty much a copy of the following in-tree zephyr example, with overlays for the [OnSemi FUSB307](https://www.onsemi.com/products/interfaces/usb-type-c/fusb307b) TCPC (Type-C Port Controller)
https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/subsys/usb_c/sink

Please note, consistent with the above example the build_rdo() will build a Request Data Object and select object position 1 (5V). It doesn't build the RDO from the Source Capabilities stored in the dpm_data object.

## Example Output

Output when connected to [UGREEN Model CD244 65W GaN Fast Charger](https://www.ugreen.com/products/65w-3-ports-gan-fast-charger?variant=39915659591742):

```
[00:00:00.251,586] <err> usbc_stack: Couldn't disable vconn: -5
[00:00:00.251,617] <inf> usbc_stack: ErrorRecovery
[00:00:00.251,678] <inf> usbc_stack: Disabled
[00:00:00.251,678] <inf> usbc_stack: PE_SUSPEND
[00:00:00.251,678] <inf> usbc_stack: PRL_HR_SUSPEND
[00:00:00.251,678] <inf> usbc_stack: PRL_TX_SUSPEND
*** Booting Zephyr OS build v4.1.0-5062-g450345aea3bf ***
[00:00:00.251,770] <inf> main: Number of Sink Capabilies 4
[00:00:00.251,770] <inf> main: Capability = 0002D00A
[00:00:00.251,770] <inf> main: Starting the USB-C Subsystem
[00:00:00.251,831] <err> usbc_stack: TCPC initialization failed: -11
[00:00:00.256,652] <inf> tcpc_fusb307: Initializing FUSB307 chip: fusb307_tcpc0@50
[00:00:00.256,988] <err> usbc_stack: TCPC initialization failed: -11
[00:00:00.259,033] <inf> tcpc_fusb307: Initialized chip is: 0779:0133:0202
[00:00:00.262,207] <inf> tcpc_fusb307: FUSB307 TCPC initialized
[00:00:00.262,207] <inf> usbc_stack: ErrorRecovery
[00:00:00.264,038] <dbg> tcpc_fusb307: fusb307_tcpc_get_cc: CC changed values: 0->7, 0->0
[00:00:00.270,751] <dbg> tcpc_fusb307: fusb307_tcpc_get_cc: CC changed values: 7->7, 0->0
[00:00:00.506,134] <inf> usbc_stack: Unattached.SNK
[00:00:00.507,324] <dbg> tcpc_fusb307: fusb307_tcpc_get_cc: CC changed values: 7->7, 0->0
[00:00:00.508,331] <inf> usbc_stack: AttachWait.SNK
[00:00:00.712,890] <inf> usbc_stack: Attached.SNK
[00:00:00.717,041] <inf> usbc_stack: PE_SNK_Startup
[00:00:00.717,071] <inf> main: No PD Explicit Contract is in place
[00:00:00.717,071] <inf> usbc_stack: PRL_INIT
[00:00:00.717,071] <inf> usbc_stack: PRL_HR_Wait_for_Request
[00:00:00.717,102] <inf> usbc_stack: PRL_Tx_PHY_Layer_Reset
[00:00:00.717,529] <inf> usbc_stack: PRL_Tx_Wait_for_Message_Request
[00:00:00.718,688] <dbg> tcpc_fusb307: fusb307_tcpc_get_cc: CC changed values: 7->7, 0->0
[00:00:00.719,360] <inf> usbc_stack: PE_SNK_Discovery
[00:00:00.721,557] <inf> usbc_stack: PE_SNK_Wait_For_Capabilities
[00:00:00.739,990] <inf> main: PWR 3A0: Sink SubPower state a 5V / 3A
[00:00:01.344,726] <inf> usbc_stack: PE_SNK_Hard_Reset
[00:00:01.344,757] <inf> usbc_stack: PRL_HR_Reset_Layer
[00:00:01.345,184] <inf> usbc_stack: PRL_Tx_PHY_Layer_Reset
[00:00:01.345,611] <inf> usbc_stack: PRL_Tx_Wait_for_Message_Request
[00:00:01.346,038] <inf> usbc_stack: PRL_HR_Wait_for_PHY_Hard_Reset_Complete
[00:00:01.353,149] <inf> usbc_stack: PRL_HR_Wait_For_PE_Hard_Reset_Complete
[00:00:01.358,947] <inf> usbc_stack: PE_SNK_Transition_to_default
[00:00:01.358,978] <inf> main: Transition the Sink to default
[00:00:01.358,978] <inf> main: Data Role has been set to Upstream Facing Port (UFP)
[00:00:01.359,680] <inf> usbc_stack: PE_SNK_Startup
[00:00:01.359,710] <inf> main: No PD Explicit Contract is in place
[00:00:01.359,710] <inf> usbc_stack: PRL_INIT
[00:00:01.359,710] <inf> usbc_stack: PRL_HR_Wait_for_Request
[00:00:01.359,741] <inf> usbc_stack: PRL_Tx_PHY_Layer_Reset
[00:00:01.360,137] <inf> usbc_stack: PRL_Tx_Wait_for_Message_Request
[00:00:01.360,839] <inf> usbc_stack: PE_SNK_Discovery
[00:00:01.363,128] <inf> usbc_stack: PE_SNK_Wait_For_Capabilities
[00:00:01.378,509] <dbg> tcpc_fusb307: fusb307_alert_work_cb: power status: 08
[00:00:01.386,505] <inf> usbc_stack: PRL_TX_SUSPEND
[00:00:01.386,505] <inf> usbc_stack: PRL_HR_SUSPEND
[00:00:01.386,535] <inf> usbc_stack: PE_SUSPEND
[00:00:01.387,512] <inf> usbc_stack: Unattached.SNK
[00:00:01.389,068] <dbg> tcpc_fusb307: fusb307_tcpc_get_cc: CC changed values: 7->7, 0->0
[00:00:01.390,045] <inf> usbc_stack: AttachWait.SNK
[00:00:01.393,249] <dbg> tcpc_fusb307: fusb307_tcpc_get_cc: CC changed values: 7->7, 0->0
[00:00:02.303,588] <dbg> tcpc_fusb307: fusb307_alert_work_cb: power status: 0c
[00:00:02.304,382] <inf> usbc_stack: Attached.SNK
[00:00:02.309,692] <inf> usbc_stack: PE_SNK_Startup
[00:00:02.309,692] <inf> main: No PD Explicit Contract is in place
[00:00:02.309,722] <inf> usbc_stack: PRL_INIT
[00:00:02.309,722] <inf> usbc_stack: PRL_HR_Wait_for_Request
[00:00:02.309,722] <inf> usbc_stack: PRL_Tx_PHY_Layer_Reset
[00:00:02.310,150] <inf> usbc_stack: PRL_Tx_Wait_for_Message_Request
[00:00:02.311,340] <dbg> tcpc_fusb307: fusb307_tcpc_get_cc: CC changed values: 7->7, 0->0
[00:00:02.312,072] <inf> usbc_stack: PE_SNK_Discovery
[00:00:02.314,239] <inf> usbc_stack: PE_SNK_Wait_For_Capabilities
[00:00:02.332,702] <inf> main: PWR 3A0: Sink SubPower state a 5V / 3A
[00:00:02.423,797] <dbg> tcpc_fusb307: fusb307_alert_work_cb: MSG pending
[00:00:02.428,039] <inf> usbc_stack: RECV 61a1/6
[00:00:02.428,070] <inf> usbc_stack:    [0]0801912c
[00:00:02.428,070] <inf> usbc_stack:    [1]0002d12c
[00:00:02.428,070] <inf> usbc_stack:    [2]0003c12c
[00:00:02.428,070] <inf> usbc_stack:    [3]0004b12c
[00:00:02.428,100] <inf> usbc_stack:    [4]0006412c
[00:00:02.428,100] <inf> usbc_stack:    [5]c8dc213c
[00:00:02.429,229] <inf> usbc_stack: PE_SNK_Evaluate_Capability
[00:00:02.429,260] <inf> main: Source Capabilities Received
[00:00:02.429,260] <inf> usbc_stack: PE_SNK_Select_Capability
[00:00:02.429,290] <inf> main: port0_policy_cb_get_rdo()
[00:00:02.429,290] <inf> main: A PD Explicit Contract is in place
[00:00:02.431,243] <inf> usbc_stack: PRL_Tx_Wait_for_PHY_response
[00:00:02.433,319] <inf> usbc_stack: PRL_Tx_Wait_for_Message_Request
[00:00:02.437,957] <dbg> tcpc_fusb307: fusb307_alert_work_cb: MSG pending
[00:00:02.438,842] <inf> usbc_stack: RECV 03a3/0
[00:00:02.440,002] <inf> usbc_stack: PE_SNK_Transition_Sink
[00:00:02.565,673] <dbg> tcpc_fusb307: fusb307_alert_work_cb: MSG pending
[00:00:02.566,589] <inf> usbc_stack: RECV 05a6/0
[00:00:02.567,749] <inf> main: Transition the Power Supply
[00:00:02.567,749] <inf> usbc_stack: PE_SNK_Ready
[00:00:03.252,044] <inf> main: Source Caps:
[00:00:03.252,044] <inf> main: PDO 0:
[00:00:03.252,075] <inf> main:  Type:              FIXED
[00:00:03.252,075] <inf> main:  Current:           3000
[00:00:03.252,075] <inf> main:  Voltage:           5000
[00:00:03.252,075] <inf> main:  Peak Current:      0
[00:00:03.252,105] <inf> main:  Uchunked Support:  0
[00:00:03.252,105] <inf> main:  Dual Role Data:    0
[00:00:03.252,136] <inf> main:  USB Comms:         0
[00:00:03.252,136] <inf> main:  Unconstrained Pwr: 1
[00:00:03.252,136] <inf> main:  USB Suspend:       0
[00:00:03.252,136] <inf> main:  Dual Role Power:   0
[00:00:03.302,246] <inf> main: PDO 1:
[00:00:03.302,246] <inf> main:  Type:              FIXED
[00:00:03.302,246] <inf> main:  Current:           3000
[00:00:03.302,276] <inf> main:  Voltage:           9000
[00:00:03.302,276] <inf> main:  Peak Current:      0
[00:00:03.302,276] <inf> main:  Uchunked Support:  0
[00:00:03.302,307] <inf> main:  Dual Role Data:    0
[00:00:03.302,307] <inf> main:  USB Comms:         0
[00:00:03.302,307] <inf> main:  Unconstrained Pwr: 0
[00:00:03.302,307] <inf> main:  USB Suspend:       0
[00:00:03.302,337] <inf> main:  Dual Role Power:   0
[00:00:03.352,416] <inf> main: PDO 2:
[00:00:03.352,416] <inf> main:  Type:              FIXED
[00:00:03.352,416] <inf> main:  Current:           3000
[00:00:03.352,447] <inf> main:  Voltage:           12000
[00:00:03.352,447] <inf> main:  Peak Current:      0
[00:00:03.352,447] <inf> main:  Uchunked Support:  0
[00:00:03.352,478] <inf> main:  Dual Role Data:    0
[00:00:03.352,478] <inf> main:  USB Comms:         0
[00:00:03.352,478] <inf> main:  Unconstrained Pwr: 0
[00:00:03.352,478] <inf> main:  USB Suspend:       0
[00:00:03.352,508] <inf> main:  Dual Role Power:   0
[00:00:03.402,587] <inf> main: PDO 3:
[00:00:03.402,587] <inf> main:  Type:              FIXED
[00:00:03.402,587] <inf> main:  Current:           3000
[00:00:03.402,618] <inf> main:  Voltage:           15000
[00:00:03.402,618] <inf> main:  Peak Current:      0
[00:00:03.402,618] <inf> main:  Uchunked Support:  0
[00:00:03.402,618] <inf> main:  Dual Role Data:    0
[00:00:03.402,648] <inf> main:  USB Comms:         0
[00:00:03.402,648] <inf> main:  Unconstrained Pwr: 0
[00:00:03.402,648] <inf> main:  USB Suspend:       0
[00:00:03.402,648] <inf> main:  Dual Role Power:   0
[00:00:03.452,758] <inf> main: PDO 4:
[00:00:03.452,758] <inf> main:  Type:              FIXED
[00:00:03.452,758] <inf> main:  Current:           3000
[00:00:03.452,789] <inf> main:  Voltage:           20000
[00:00:03.452,789] <inf> main:  Peak Current:      0
[00:00:03.452,789] <inf> main:  Uchunked Support:  0
[00:00:03.452,789] <inf> main:  Dual Role Data:    0
[00:00:03.452,819] <inf> main:  USB Comms:         0
[00:00:03.452,819] <inf> main:  Unconstrained Pwr: 0
[00:00:03.452,819] <inf> main:  USB Suspend:       0
[00:00:03.452,850] <inf> main:  Dual Role Power:   0
[00:00:03.502,929] <inf> main: PDO 5:
[00:00:03.502,960] <inf> main:  Type:              AUGMENTED
[00:00:03.502,960] <inf> main:  Min Voltage:       3300
[00:00:03.502,960] <inf> main:  Max Voltage:       11000
[00:00:03.502,960] <inf> main:  Max Current:       3000
[00:00:03.502,990] <inf> main:  PPS Power Limited: 1
```
