	component Edge_Detection_Subsystem is
		port (
			edge_detection_control_slave_address    : in  std_logic_vector(1 downto 0)  := (others => 'X'); -- address
			edge_detection_control_slave_write_n    : in  std_logic                     := 'X';             -- write_n
			edge_detection_control_slave_writedata  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- writedata
			edge_detection_control_slave_chipselect : in  std_logic                     := 'X';             -- chipselect
			edge_detection_control_slave_readdata   : out std_logic_vector(31 downto 0);                    -- readdata
			sys_clk_clk                             : in  std_logic                     := 'X';             -- clk
			sys_reset_reset_n                       : in  std_logic                     := 'X';             -- reset_n
			video_stream_sink_data                  : in  std_logic_vector(23 downto 0) := (others => 'X'); -- data
			video_stream_sink_startofpacket         : in  std_logic                     := 'X';             -- startofpacket
			video_stream_sink_endofpacket           : in  std_logic                     := 'X';             -- endofpacket
			video_stream_sink_valid                 : in  std_logic                     := 'X';             -- valid
			video_stream_sink_ready                 : out std_logic;                                        -- ready
			video_stream_source_ready               : in  std_logic                     := 'X';             -- ready
			video_stream_source_data                : out std_logic_vector(23 downto 0);                    -- data
			video_stream_source_startofpacket       : out std_logic;                                        -- startofpacket
			video_stream_source_endofpacket         : out std_logic;                                        -- endofpacket
			video_stream_source_valid               : out std_logic                                         -- valid
		);
	end component Edge_Detection_Subsystem;

	u0 : component Edge_Detection_Subsystem
		port map (
			edge_detection_control_slave_address    => CONNECTED_TO_edge_detection_control_slave_address,    -- edge_detection_control_slave.address
			edge_detection_control_slave_write_n    => CONNECTED_TO_edge_detection_control_slave_write_n,    --                             .write_n
			edge_detection_control_slave_writedata  => CONNECTED_TO_edge_detection_control_slave_writedata,  --                             .writedata
			edge_detection_control_slave_chipselect => CONNECTED_TO_edge_detection_control_slave_chipselect, --                             .chipselect
			edge_detection_control_slave_readdata   => CONNECTED_TO_edge_detection_control_slave_readdata,   --                             .readdata
			sys_clk_clk                             => CONNECTED_TO_sys_clk_clk,                             --                      sys_clk.clk
			sys_reset_reset_n                       => CONNECTED_TO_sys_reset_reset_n,                       --                    sys_reset.reset_n
			video_stream_sink_data                  => CONNECTED_TO_video_stream_sink_data,                  --            video_stream_sink.data
			video_stream_sink_startofpacket         => CONNECTED_TO_video_stream_sink_startofpacket,         --                             .startofpacket
			video_stream_sink_endofpacket           => CONNECTED_TO_video_stream_sink_endofpacket,           --                             .endofpacket
			video_stream_sink_valid                 => CONNECTED_TO_video_stream_sink_valid,                 --                             .valid
			video_stream_sink_ready                 => CONNECTED_TO_video_stream_sink_ready,                 --                             .ready
			video_stream_source_ready               => CONNECTED_TO_video_stream_source_ready,               --          video_stream_source.ready
			video_stream_source_data                => CONNECTED_TO_video_stream_source_data,                --                             .data
			video_stream_source_startofpacket       => CONNECTED_TO_video_stream_source_startofpacket,       --                             .startofpacket
			video_stream_source_endofpacket         => CONNECTED_TO_video_stream_source_endofpacket,         --                             .endofpacket
			video_stream_source_valid               => CONNECTED_TO_video_stream_source_valid                --                             .valid
		);

