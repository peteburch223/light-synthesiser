# https://github.com/hparra/ruby-serialport

  require "serialport"

  #params for serial port

  port_str_ary = `ls /dev`.split("\n").grep(/usb|ACM/i).map{|d| "/dev/#{d}"}
  port_str = port_str_ary[0]
  baud_rate = 115200
  data_bits = 8
  stop_bits = 1
  parity = SerialPort::NONE

  sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)
  sp.read_timeout =1000

  #just read forever
  j = 0
  while true do

    red = j%255
    blue = (j+1)%255
    green = (j+2)%255

    request = "#{red},#{blue},#{green}\n"
    expect = "#{red.to_s(16)}#{blue.to_s(16)}#{green.to_s(16)}\n".upcase

    sp.write request
    response = sp.gets
    if response = expect
      p response
      j+= 1
    else
      puts 'incorrect response'
    end
    break if j==512

  end

  sp.close
