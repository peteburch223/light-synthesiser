require 'Nokogiri'

class XmlReader

  attr_reader :doc

  def initialize(file_instance=File)
    @file_instance = file_instance
  end

  def read_xml(file)
    @doc = @file_instance.open(file) { |f| Nokogiri::XML(f) }
  end

  def extract_parameters
    @parameters ={}
    @doc.xpath('/parameters/calibration/*').each do |element|
      temp = {}
      element.attributes.each {|k, v| temp[k.to_sym] = v.value.to_i}
      @parameters[element.name.to_sym] = temp
    end
    @parameters
  end

  def extract_colours
    @colours = []
    @doc.xpath('/parameters/colours/*').each do |colour|
      component = {}
      colour.children.each do |comp|
        if comp.is_a?Nokogiri::XML::Element
          temp = {}
          comp.attributes.each{|k,v| temp[k.to_sym] = v.value.to_i}
          component[comp.name.to_sym] = temp
        end
      end
      @colours << component
    end
    @colours
  end
end
