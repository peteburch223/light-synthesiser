require 'arduino_serial'
require 'serialport'

describe ArduinoSerial do

  let(:serial){ double SerialPort, write:nil, gets:nil}
  let(:test_string){ "DDDD,EEEE,FFFF\n"}
  let(:expect_string){ "DDDDEEEEFFFFOK\n"}

  let(:colour_1) { double Colour, red_time:4875, green_time:5250, blue_time:9750}
  let(:colour_2) { double Colour, red_time:4875, green_time:5250, blue_time:9750}
  let(:colour_3) { double Colour, red_time:4875, green_time:5250, blue_time:9750}
  let(:colour_4) { double Colour, red_time:4875, green_time:5250, blue_time:9750}

  let(:colours) { [colour_1, colour_2, colour_3, colour_4]}

  subject(:arduino_serial){ described_class.new(serial)}


  describe '#send' do
    it 'calls #write method of serialport' do
      arduino_serial.send(test_string)
      expect(serial).to have_received(:write).with(test_string)
    end
    it 'returns true when expected response received' do
      allow(serial).to receive(:gets).and_return(expect_string)
      expect(arduino_serial.send(test_string)).to eq true
    end

    it 'returns false when expected response not received' do
      allow(serial).to receive(:gets).and_return('')
      expect(arduino_serial.send(test_string)).to eq false
    end
  end


  describe '#send_colour_frame' do
    it 'sends a correctly framed message with the correct number of messages' do
      arduino_serial.send_colour_frame(colours)
      expect(serial).to receive(:write).exactly(6).times
    end
  end

  describe '#send_colour' do
    it 'sends a message header of 0xFFFFFF' do
    end

    it 'sends a colour of three CSV' do
    end
  end

end
