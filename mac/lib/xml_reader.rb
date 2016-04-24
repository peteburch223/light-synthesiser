require 'Nokogiri'

class XmlReader

  attr_reader :doc

  def initialize(file_instance=File)
    @file_instance = file_instance
  end

  def read_xml(file)
    @doc = @file_instance.open(file) { |f| Nokogiri::XML(f) }
  end

  def settings
    @doc.xpath("/parameters/settings/*").reduce({}) do |result, element|
      result.merge(element.name.to_sym => extract(element.attributes))
    end
  end

  def colours
    @doc.xpath("/parameters/colours/*").map do |element|
      extract(element.attributes)
    end
  end

  private

  def extract(attributes)
    attributes.reduce({}) do |result, (k,v)| # |k, v|
      result.merge(k.to_sym => v.value.to_f)
    end

  end
end
