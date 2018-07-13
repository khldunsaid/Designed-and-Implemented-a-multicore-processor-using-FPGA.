//Legal Notice: (C)2017 Altera Corporation. All rights reserved.  Your
//use of Altera Corporation's design tools, logic functions and other
//software and tools, and its AMPP partner logic functions, and any
//output files any of the foregoing (including device programming or
//simulation files), and any associated documentation or information are
//expressly subject to the terms and conditions of the Altera Program
//License Subscription Agreement or other applicable license agreement,
//including, without limitation, that your use is for the sole purpose
//of programming logic devices manufactured by Altera and sold by Altera
//or its authorized distributors.  Please refer to the applicable
//agreement for further details.

// synthesis translate_off
`timescale 1ns / 1ps
// synthesis translate_on

// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module multicore_performance_counter (
                                       // inputs:
                                        address,
                                        begintransfer,
                                        clk,
                                        reset_n,
                                        write,
                                        writedata,

                                       // outputs:
                                        readdata
                                     )
;

  output  [ 31: 0] readdata;
  input   [  3: 0] address;
  input            begintransfer;
  input            clk;
  input            reset_n;
  input            write;
  input   [ 31: 0] writedata;

  wire             clk_en;
  reg     [ 63: 0] event_counter_0;
  reg     [ 63: 0] event_counter_1;
  reg     [ 63: 0] event_counter_2;
  reg     [ 63: 0] event_counter_3;
  wire             global_enable;
  wire             global_reset;
  wire             go_strobe_0;
  wire             go_strobe_1;
  wire             go_strobe_2;
  wire             go_strobe_3;
  wire    [ 31: 0] read_mux_out;
  reg     [ 31: 0] readdata;
  wire             stop_strobe_0;
  wire             stop_strobe_1;
  wire             stop_strobe_2;
  wire             stop_strobe_3;
  reg     [ 63: 0] time_counter_0;
  reg     [ 63: 0] time_counter_1;
  reg     [ 63: 0] time_counter_2;
  reg     [ 63: 0] time_counter_3;
  reg              time_counter_enable_0;
  reg              time_counter_enable_1;
  reg              time_counter_enable_2;
  reg              time_counter_enable_3;
  wire             write_strobe;
  //control_slave, which is an e_avalon_slave
  assign clk_en = -1;
  assign write_strobe = write & begintransfer;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          time_counter_0 <= 0;
      else if ((time_counter_enable_0 & global_enable) | global_reset)
          if (global_reset)
              time_counter_0 <= 0;
          else 
            time_counter_0 <= time_counter_0 + 1;
    end


  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          event_counter_0 <= 0;
      else if ((go_strobe_0 & global_enable) | global_reset)
          if (global_reset)
              event_counter_0 <= 0;
          else 
            event_counter_0 <= event_counter_0 + 1;
    end


  assign stop_strobe_0 = (address == 0) && write_strobe;
  assign go_strobe_0 = (address == 1) && write_strobe;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          time_counter_enable_0 <= 0;
      else if (clk_en)
          if (stop_strobe_0 | global_reset)
              time_counter_enable_0 <= 0;
          else if (go_strobe_0)
              time_counter_enable_0 <= -1;
    end


  assign global_enable = time_counter_enable_0 | go_strobe_0;
  assign global_reset = stop_strobe_0 && writedata[0];
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          time_counter_1 <= 0;
      else if ((time_counter_enable_1 & global_enable) | global_reset)
          if (global_reset)
              time_counter_1 <= 0;
          else 
            time_counter_1 <= time_counter_1 + 1;
    end


  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          event_counter_1 <= 0;
      else if ((go_strobe_1 & global_enable) | global_reset)
          if (global_reset)
              event_counter_1 <= 0;
          else 
            event_counter_1 <= event_counter_1 + 1;
    end


  assign stop_strobe_1 = (address == 4) && write_strobe;
  assign go_strobe_1 = (address == 5) && write_strobe;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          time_counter_enable_1 <= 0;
      else if (clk_en)
          if (stop_strobe_1 | global_reset)
              time_counter_enable_1 <= 0;
          else if (go_strobe_1)
              time_counter_enable_1 <= -1;
    end


  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          time_counter_2 <= 0;
      else if ((time_counter_enable_2 & global_enable) | global_reset)
          if (global_reset)
              time_counter_2 <= 0;
          else 
            time_counter_2 <= time_counter_2 + 1;
    end


  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          event_counter_2 <= 0;
      else if ((go_strobe_2 & global_enable) | global_reset)
          if (global_reset)
              event_counter_2 <= 0;
          else 
            event_counter_2 <= event_counter_2 + 1;
    end


  assign stop_strobe_2 = (address == 8) && write_strobe;
  assign go_strobe_2 = (address == 9) && write_strobe;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          time_counter_enable_2 <= 0;
      else if (clk_en)
          if (stop_strobe_2 | global_reset)
              time_counter_enable_2 <= 0;
          else if (go_strobe_2)
              time_counter_enable_2 <= -1;
    end


  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          time_counter_3 <= 0;
      else if ((time_counter_enable_3 & global_enable) | global_reset)
          if (global_reset)
              time_counter_3 <= 0;
          else 
            time_counter_3 <= time_counter_3 + 1;
    end


  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          event_counter_3 <= 0;
      else if ((go_strobe_3 & global_enable) | global_reset)
          if (global_reset)
              event_counter_3 <= 0;
          else 
            event_counter_3 <= event_counter_3 + 1;
    end


  assign stop_strobe_3 = (address == 12) && write_strobe;
  assign go_strobe_3 = (address == 13) && write_strobe;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          time_counter_enable_3 <= 0;
      else if (clk_en)
          if (stop_strobe_3 | global_reset)
              time_counter_enable_3 <= 0;
          else if (go_strobe_3)
              time_counter_enable_3 <= -1;
    end


  assign read_mux_out = ({32 {(address == 0)}} & time_counter_0[31 : 0]) |
    ({32 {(address == 1)}} & time_counter_0[63 : 32]) |
    ({32 {(address == 2)}} & event_counter_0) |
    ({32 {(address == 4)}} & time_counter_1[31 : 0]) |
    ({32 {(address == 5)}} & time_counter_1[63 : 32]) |
    ({32 {(address == 6)}} & event_counter_1) |
    ({32 {(address == 8)}} & time_counter_2[31 : 0]) |
    ({32 {(address == 9)}} & time_counter_2[63 : 32]) |
    ({32 {(address == 10)}} & event_counter_2) |
    ({32 {(address == 12)}} & time_counter_3[31 : 0]) |
    ({32 {(address == 13)}} & time_counter_3[63 : 32]) |
    ({32 {(address == 14)}} & event_counter_3);

  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          readdata <= 0;
      else if (clk_en)
          readdata <= read_mux_out;
    end



endmodule

