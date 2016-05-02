class ArduinoSerial

  CONFIRM_STRING = 'OK'
  START_MESSAGE = 'z'
  END_MESSAGE = 'x'
  TERMINATOR = "\n"

  def initialize(serial_port_instance)
    @serial = serial_port_instance
  end

  def send(string, expected_response = nil)
    puts "Sending: #{string}"
    expected_response ||= (string.slice(0..-2)+",#{CONFIRM_STRING}#{TERMINATOR}").gsub(',','')
    puts "Expecting: #{expected_response}"
    @serial.write("string#{TERMINATOR}")
    expected_response == @serial.gets
  end

  def send_colour_frame(colours)
    send_frame_start
    colours.each do |colour|
      send_colour(colour)
    end
    send_frame_end
  end

  private

  def send_frame_start
    send START_MESSAGE
  end

  def send_frame_end
    send END_MESSAGE
  end

  def send_colour(colour)
    red_hex = colour.red_time.to_s(16)
    green_hex = colour.green_time.to_s(16)
    blue_hex = colour.blue_time.to_s(16)
    expected_response = "#{red_hex}#{green_hex}#{blue_hex}".upcase
    request = "#{colour.red_time},#{colour.green_time},#{colour.blue_time}"
    send(request, expected_response)
  end
end
