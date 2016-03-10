require 'xml_reader'

describe XmlReader do

  subject(:xml_reader){described_class.new(File)}
  let(:file_class){double File, new: file_instance}
  let(:file_instance){double :file_instance, open: nil}

  it {is_expected.to respond_to(:read_xml).with(1).argument}
  it {is_expected.to respond_to(:extract_parameters)}

  context 'reads parameters' do
    before do
      test_file = './xml/test.xml'
      xml_reader.read_xml(test_file)
    end

    it 'reads an xml file into doc' do
      expect(xml_reader.doc).to be_instance_of(Nokogiri::XML::Document)
    end

    it 'extracts configuration parameters as a hash' do
      param_hash = {:red => {:a => 1, :H => 10}, :green => {:a => 2, :H => 20}, :blue => {:a => 3, :H => 30}}
      expect(xml_reader.extract_parameters).to eq param_hash
    end
  end

  context 'reads colours' do
    it 'extracts colours as an array of hashes' do
      colour_array = [{:red => {:hue => 1, :saturation => 10, :brightness => 100},
                       :green => {:hue => 2, :saturation => 20, :brightness => 200},
                       :blue => {:hue => 3, :saturation => 30, :brightness => 300}},
                      {:red => {:hue => 4, :saturation => 40, :brightness => 400},
                       :green => {:hue => 5, :saturation => 50, :brightness => 500},
                       :blue => {:hue => 6, :saturation => 60, :brightness => 600}}]

      test_file = './xml/test.xml'
      xml_reader.read_xml(test_file)
      expect(xml_reader.extract_colours).to eq colour_array
    end

  end


end
