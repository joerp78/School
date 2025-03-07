	Edge_Detection_Subsystem u0 (
		.edge_detection_control_slave_address    (<connected-to-edge_detection_control_slave_address>),    // edge_detection_control_slave.address
		.edge_detection_control_slave_write_n    (<connected-to-edge_detection_control_slave_write_n>),    //                             .write_n
		.edge_detection_control_slave_writedata  (<connected-to-edge_detection_control_slave_writedata>),  //                             .writedata
		.edge_detection_control_slave_chipselect (<connected-to-edge_detection_control_slave_chipselect>), //                             .chipselect
		.edge_detection_control_slave_readdata   (<connected-to-edge_detection_control_slave_readdata>),   //                             .readdata
		.sys_clk_clk                             (<connected-to-sys_clk_clk>),                             //                      sys_clk.clk
		.sys_reset_reset_n                       (<connected-to-sys_reset_reset_n>),                       //                    sys_reset.reset_n
		.video_stream_sink_data                  (<connected-to-video_stream_sink_data>),                  //            video_stream_sink.data
		.video_stream_sink_startofpacket         (<connected-to-video_stream_sink_startofpacket>),         //                             .startofpacket
		.video_stream_sink_endofpacket           (<connected-to-video_stream_sink_endofpacket>),           //                             .endofpacket
		.video_stream_sink_valid                 (<connected-to-video_stream_sink_valid>),                 //                             .valid
		.video_stream_sink_ready                 (<connected-to-video_stream_sink_ready>),                 //                             .ready
		.video_stream_source_ready               (<connected-to-video_stream_source_ready>),               //          video_stream_source.ready
		.video_stream_source_data                (<connected-to-video_stream_source_data>),                //                             .data
		.video_stream_source_startofpacket       (<connected-to-video_stream_source_startofpacket>),       //                             .startofpacket
		.video_stream_source_endofpacket         (<connected-to-video_stream_source_endofpacket>),         //                             .endofpacket
		.video_stream_source_valid               (<connected-to-video_stream_source_valid>)                //                             .valid
	);

