onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_wen_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_req_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_gnt_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_addr_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_be_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_wdata_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_rvalid_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_rdata_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_wen_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_req_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_gnt_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_addr_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_wdata_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_be_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_rvalid_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_rdata_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/dft_test_mode_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/dft_cg_enable_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sys_clk_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sys_resetn_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/periph_clk_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_paddr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_pwdata
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_pwrite
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_psel
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_penable
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_prdata
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_pready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_pslverr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/events_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/event_valid_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/event_data_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/event_ready_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_clk
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_csn
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_oen
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_sdo
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_sdi
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_scl_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_scl_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_scl_oe
add wave -noupdate -group udma_subsystem -expand /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_sda_i
add wave -noupdate -group udma_subsystem -expand /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_sda_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_sda_oe
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/cam_clk_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/cam_data_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/cam_hsync_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/cam_vsync_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/uart_rx_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/uart_tx_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_clk_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_cmd_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_cmd_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_cmd_oen_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_data_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_data_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_data_oen_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sd0_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sd1_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_ws_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_ws_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_ws_oe
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sck_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sck_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sck_oe
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_cs_no
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_ck_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_ck_no
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_rwds_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_rwds_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_rwds_oe_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_dq_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_dq_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_dq_oe_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_reset_no
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_startaddr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_size
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_continuous
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_en
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_clr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_req
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_gnt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_destination
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_events
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_en
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_pending
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_curr_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_bytes_left
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_startaddr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_size
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_continuous
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_en
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_clr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_stream
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_stream_id
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_destination
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_events
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_en
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_pending
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_curr_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_bytes_left
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_destination
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_stream
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_stream_id
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_sot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_eot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_req
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_destination
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_gnt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_sot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_eot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_events
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rf_event
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_clk_periphs_core
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_clk_periphs_per
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_data_to
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_rwn
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_data_from
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_spi_eot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_wen_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_req_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_gnt_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_addr_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_be_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_wdata_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_rvalid_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_ro_rdata_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_wen_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_req_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_gnt_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_addr_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_wdata_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_be_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_rvalid_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/L2_wo_rdata_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/dft_test_mode_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/dft_cg_enable_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sys_clk_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sys_resetn_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/periph_clk_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_paddr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_pwdata
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_pwrite
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_psel
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_penable
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_prdata
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_pready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/udma_apb_pslverr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/events_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/event_valid_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/event_data_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/event_ready_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_clk
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_csn
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_oen
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_sdo
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/spi_sdi
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_scl_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_scl_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_scl_oe
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_sda_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_sda_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2c_sda_oe
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/cam_clk_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/cam_data_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/cam_hsync_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/cam_vsync_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/uart_rx_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/uart_tx_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_clk_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_cmd_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_cmd_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_cmd_oen_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_data_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_data_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/sdio_data_oen_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sd0_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sd1_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_ws_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_ws_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_ws_oe
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sck_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sck_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i2s_slave_sck_oe
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_cs_no
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_ck_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_ck_no
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_rwds_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_rwds_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_rwds_oe_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_dq_i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_dq_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_dq_oe_o
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/hyper_reset_no
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_startaddr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_size
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_continuous
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_en
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_cfg_clr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_req
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_gnt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_destination
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_events
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_en
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_pending
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_curr_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ch_bytes_left
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_startaddr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_size
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_continuous
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_en
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_clr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_stream
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_cfg_stream_id
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_destination
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_events
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_en
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_pending
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_curr_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ch_bytes_left
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_destination
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_stream
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_stream_id
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_sot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_eot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rx_ext_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_req
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_destination
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_gnt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_tx_ext_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_data
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_datasize
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_sot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_eot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_stream_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_events
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_rf_event
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_clk_periphs_core
add wave -noupdate -group udma_subsystem -expand /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_clk_periphs_per
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_data_to
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_addr
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_rwn
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_data_from
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_valid
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_periph_ready
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_spi_eot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_i2c_err
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_i2c_eot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_i2c_nack
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_uart_evt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_trigger_events
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_cam_evt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_i2s_evt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_filter_eot_evt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_filter_act_evt
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_hyper_sys_clk
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_hyper_periph_clk
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_evt_eot_hyper
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/is_hyper_read_q
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/is_hyper_read_d
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_sdio_eot
add wave -noupdate -group udma_subsystem /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/s_sdio_err
add wave -noupdate -group i2c_mem_0 /tb_pulp/genblk7/i_i2c_mem_0/A0
add wave -noupdate -group i2c_mem_0 /tb_pulp/genblk7/i_i2c_mem_0/A1
add wave -noupdate -group i2c_mem_0 /tb_pulp/genblk7/i_i2c_mem_0/A2
add wave -noupdate -group i2c_mem_0 /tb_pulp/genblk7/i_i2c_mem_0/WP
add wave -noupdate -group i2c_mem_0 /tb_pulp/genblk7/i_i2c_mem_0/SDA
add wave -noupdate -group i2c_mem_0 /tb_pulp/genblk7/i_i2c_mem_0/SCL
add wave -noupdate -group i2c_mem_0 /tb_pulp/genblk7/i_i2c_mem_0/RESET
add wave -noupdate -group i2c_mem_1 /tb_pulp/genblk7/i_i2c_mem_1/A0
add wave -noupdate -group i2c_mem_1 /tb_pulp/genblk7/i_i2c_mem_1/A1
add wave -noupdate -group i2c_mem_1 /tb_pulp/genblk7/i_i2c_mem_1/A2
add wave -noupdate -group i2c_mem_1 /tb_pulp/genblk7/i_i2c_mem_1/WP
add wave -noupdate -group i2c_mem_1 /tb_pulp/genblk7/i_i2c_mem_1/SDA
add wave -noupdate -group i2c_mem_1 /tb_pulp/genblk7/i_i2c_mem_1/SCL
add wave -noupdate -group i2c_mem_1 /tb_pulp/genblk7/i_i2c_mem_1/RESET
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/sys_clk_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/periph_clk_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/rstn_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/ext_events_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_data_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_addr_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_valid_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rwn_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_data_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_ready_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_startaddr_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_size_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_continuous_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_en_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_clr_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_en_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_pending_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_curr_addr_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_cmd_bytes_left_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_startaddr_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_size_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_continuous_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_en_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_clr_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_en_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_pending_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_curr_addr_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_rx_bytes_left_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_startaddr_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_size_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_continuous_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_en_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_clr_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_en_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_pending_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_curr_addr_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cfg_tx_bytes_left_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cmd_req_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cmd_gnt_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cmd_datasize_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cmd_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cmd_valid_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/cmd_ready_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_tx_req_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_tx_gnt_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_tx_datasize_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_tx_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_tx_valid_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_tx_ready_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_rx_datasize_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_rx_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_rx_valid_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/data_rx_ready_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/err_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/eot_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/nack_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/scl_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/scl_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/scl_oe}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/sda_i}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/sda_o}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/sda_oe}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_tx}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_tx_valid}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_tx_ready}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_tx_dc}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_tx_dc_valid}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_tx_dc_ready}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_rx_dc}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_rx_dc_valid}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_data_rx_dc_ready}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_do_rst}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_udma_cmd}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_udma_cmd_valid}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_udma_cmd_ready}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_i2c_cmd}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_i2c_cmd_valid}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_i2c_cmd_ready}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_i2c_busy}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_i2c_al}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_i2c_eot}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_i2c_nack}
add wave -noupdate -group i2c_top_0 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[0]/i_i2c/s_events}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/sys_clk_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/periph_clk_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/rstn_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/ext_events_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_data_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_addr_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_valid_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rwn_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_data_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_ready_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_startaddr_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_size_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_continuous_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_en_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_clr_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_en_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_pending_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_curr_addr_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_cmd_bytes_left_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_startaddr_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_size_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_continuous_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_en_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_clr_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_en_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_pending_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_curr_addr_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_rx_bytes_left_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_startaddr_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_size_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_continuous_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_en_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_clr_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_en_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_pending_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_curr_addr_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cfg_tx_bytes_left_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cmd_req_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cmd_gnt_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cmd_datasize_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cmd_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cmd_valid_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/cmd_ready_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_tx_req_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_tx_gnt_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_tx_datasize_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_tx_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_tx_valid_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_tx_ready_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_rx_datasize_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_rx_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_rx_valid_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/data_rx_ready_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/err_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/eot_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/nack_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/scl_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/scl_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/scl_oe}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/sda_i}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/sda_o}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/sda_oe}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_tx}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_tx_valid}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_tx_ready}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_tx_dc}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_tx_dc_valid}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_tx_dc_ready}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_rx_dc}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_rx_dc_valid}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_data_rx_dc_ready}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_do_rst}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_udma_cmd}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_udma_cmd_valid}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_udma_cmd_ready}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_i2c_cmd}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_i2c_cmd_valid}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_i2c_cmd_ready}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_i2c_busy}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_i2c_al}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_i2c_eot}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_i2c_nack}
add wave -noupdate -expand -group i2c_top_1 {/tb_pulp/i_dut/soc_domain_i/pulp_soc_i/soc_peripherals_i/i_udma/i_i2c_gen[1]/i_i2c/s_events}
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
quietly wave cursor active 0
configure wave -namecolwidth 250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ps} {28190133657 ps}
