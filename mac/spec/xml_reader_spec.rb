require 'xml_reader'

describe XmlReader do

  subject(:xml_reader){described_class.new(File)}
  let(:file_class){double File, new: file_instance}
  let(:file_instance){double :file_instance, open: nil}

  it {is_expected.to respond_to(:read_xml).with(1).argument}

  context 'reads parameters' do
    before do
      test_file = './xml/test.xml'
      xml_reader.read_xml(test_file)
    end

    it 'reads an xml file into doc' do
      expect(xml_reader.doc).to be_instance_of(Nokogiri::XML::Document)
    end

    it 'extracts configuration parameters as a hash' do
      param_hash = {calibration: {red: 1.0, green: 2.0, blue: 3.0},
                    session: {power: 100.0, br_balance: 0.95, gm_balance: 1.05}}
      expect(xml_reader.settings).to eq param_hash
    end

    it 'reads colours as an array of hashes' do
      colour_array = [{hue:125, saturation: 10, brightness:70},
                      {hue:246, saturation: 20, brightness:50}]

      expect(xml_reader.colours).to eq colour_array
    end

    it 'generates an error if any parameters are outside of bounds' do
      
    end

  end
end
