require 'colour'

describe Colour do


  subject(:colour) { described_class.new(
                    settings:
                      {calibration: {red: 1.0, green: 1.0, blue: 1.0},
                       session: {power: 100.0, br_balance: 0.0, gm_balance: 5.0}},
                    colour_hsb:
                      {hue:240, saturation: 50, brightness:100})
                    }

  context 'colour calculation' do
    
    it 'correct for red' do
      expect(colour.red_time).to eq 4875
    end
    it 'correct for green' do
      expect(colour.green_time).to eq 5250
    end
    it 'correct for blue' do
      expect(colour.blue_time).to eq 9750
    end
  end
end
