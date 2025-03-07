
module Edge_Detection_Subsystem (
	edge_detection_control_slave_address,
	edge_detection_control_slave_write_n,
	edge_detection_control_slave_writedata,
	edge_detection_control_slave_chipselect,
	edge_detection_control_slave_readdata,
	sys_clk_clk,
	sys_reset_reset_n,
	video_stream_sink_data,
	video_stream_sink_startofpacket,
	video_stream_sink_endofpacket,
	video_stream_sink_valid,
	video_stream_sink_ready,
	video_stream_source_ready,
	video_stream_source_data,
	video_stream_source_startofpacket,
	video_stream_source_endofpacket,
	video_stream_source_valid);	

	input	[1:0]	edge_detection_control_slave_address;
	input		edge_detection_control_slave_write_n;
	input	[31:0]	edge_detection_control_slave_writedata;
	input		edge_detection_control_slave_chipselect;
	output	[31:0]	edge_detection_control_slave_readdata;
	input		sys_clk_clk;
	input		sys_reset_reset_n;
	input	[23:0]	video_stream_sink_data;
	input		video_stream_sink_startofpacket;
	input		video_stream_sink_endofpacket;
	input		video_stream_sink_valid;
	output		video_stream_sink_ready;
	input		video_stream_source_ready;
	output	[23:0]	video_stream_source_data;
	output		video_stream_source_startofpacket;
	output		video_stream_source_endofpacket;
	output		video_stream_source_valid;
endmodule
