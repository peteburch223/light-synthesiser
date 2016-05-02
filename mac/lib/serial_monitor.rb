# https://github.com/hparra/ruby-serialport

  require "serialport"

  #params for serial port

  port_str_ary = `ls /dev`.split("\n").grep(/usb|ACM/i).map{|d| "/dev/#{d}"}
  port_str = port_str_ary[0]
  baud_rate = 115200
  data_bits = 8
  stop_bits = 1
  parity = SerialPort::NONE

  begin

    sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)
    sp.read_timeout = 0


    # this wait gives time for Arduino to reset. If there is no cap between
    # if there is no 10uF cap between RESET and GND, opening a serial connection
    # resets the Arduino
    sleep(2)
    puts "setup"

    # i = 0
    # while i < 3 do
    #   sp.write "\n"
    #   sp.gets
    #   i += 1
    # end




    # while true
    #   e = gets
    #   puts "entered #{e}"
    #   sp.write e
    #   puts "received #{sp.gets}"
    # end


    #just read forever

    while true do
      j = 0


      sp.write "z\n"
      puts "Frame start: #{sp.gets}"

      while true do

        red = j%255
        blue = (j+1)%255
        green = (j+2)%255

        request = "#{red},#{blue},#{green}\n"
        expect = "#{red.to_s(16)}#{blue.to_s(16)}#{green.to_s(16)}OK\n".upcase

        sp.write request
        response = sp.gets
        if response = expect
          p response
          j+= 1
        else
          puts 'incorrect response'
        end

        break if j==16

      end


    sp.write "x\n"
    puts "Frame end: #{sp.gets}"
    sleep 1


    while true
      puts sp.gets
    end




  end
  rescue => e
    puts "Failed to open as SerialPort: #{ e.inspect }"
  end

  sp.close
