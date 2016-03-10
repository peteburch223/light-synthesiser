require './lib/xml_reader'


xml_reader = XmlReader.new

xml_reader.read_xml('./xml/test.xml')
p xml_reader.extract_parameters
puts
p xml_reader.extract_colours
