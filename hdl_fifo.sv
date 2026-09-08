/*
 * File Name:   hdl_fifo.sv
 * Project:     Synchonous FIFO in SystemVerilog
 * Author:      Obi Obetta <obiuto.obetta@colorado.edu>
 * Date:        July 12, 2026
 * Description: Implements a simple synchronous FIFO
 */

module hdl_fifo #(
  parameter WIDTH = 32,
  parameter DEPTH = 16
) (
  input  logic             clk, rst_n, wr_en, rd_en,
  input  logic [WIDTH-1:0] din,
  output logic [WIDTH-1:0] dout,
  output logic             full, empty
);

  // extra MSB to tell the difference btw full and empty
  localparam PTR_WIDTH = $clog2(DEPTH);
  logic [PTR_WIDTH:0] wr_ptr, rd_ptr; 
  logic [WIDTH-1:0] mem [DEPTH-1:0];

  always_ff @(posedge clk) begin
    if (!rst_n) begin
      wr_ptr <= 0;
      rd_ptr <= 0;
      dout   <= 0;
    end else begin
      // writing (can't be full first)
      if (wr_en && !full) begin
        mem[wr_ptr[PTR_WIDTH-1:0]] <= din;
        wr_ptr <= wr_ptr + 1;
      end
      
      // reading (can't be empty first)
      if (rd_en && !empty) begin
        dout <= mem[rd_ptr[PTR_WIDTH-1:0]];
        rd_ptr <= rd_ptr + 1;
      end
    end
  end
  
  assign empty = (wr_ptr == rd_ptr);

  assign full = (wr_ptr[PTR_WIDTH] != rd_ptr[PTR_WIDTH]) && 
                (wr_ptr[PTR_WIDTH-1:0] == rd_ptr[PTR_WIDTH-1:0]);

endmodule
