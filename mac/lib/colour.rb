class Colour
  attr_reader :red_time, :green_time, :blue_time

  def initialize(settings:, colour_hsb:)
    @settings = settings
    @h = colour_hsb[:hue]
    @s = (100.0 - colour_hsb[:saturation]) / 100.0
    @b = colour_hsb[:brightness]
    @red_time = time(:red)
    @green_time = time(:green)
    @blue_time = time(:blue)
  end


  private

  def time(colour)
    calibration(colour) * power *
      br_balance(colour) * gm_balance(colour) *
      brightness * channel(colour)
  end

  def br_balance(colour)
    colour == :green ? 1.0 : 1.0 - @settings[:session][:br_balance]
  end

  def gm_balance(colour)
    gm_bal = @settings[:session][:gm_balance]
    1.0 + (gm_bal / 100) * (colour == :green ? 1.0  : -0.5)
  end

  def calibration(colour)
    @settings[:calibration][colour]
  end

  def power
    @settings[:session][:power]
  end

  def brightness
    @b
  end

  def channel(colour)
    if @h > 0 && @h <= 60
      channel_0_60(colour)
    elsif @h > 60 && @h <= 120
      channel_60_120(colour)
    elsif @h > 120 && @h <= 180
      channel_120_180(colour)
    elsif @h > 180 && @h <= 240
      channel_180_240(colour)
    elsif @h > 240 && @h <= 300
      channel_240_300(colour)
    elsif @h > 300 && @h <= 360
      channel_300_360(colour)
    end
  end


  def channel_0_60(colour)
    case colour
    when :green
      (@h / 60.0) + (1 - (@h / 60))  * @s
    when :blue
      @s
    else
      1
    end
  end

  def channel_60_120(colour)
    case colour
    when :red
      (120 - @h) / 60 + (1 - (120 - @h) / 60) * @s
    when :blue
      @s
    else
      1
    end
  end

  def channel_120_180(colour)
    case colour
    when :red
      @s
    when :blue
      (@h - 120) / 60 + (1 - (@h - 120) / 60) * @s
    else
      1
    end
  end

  def channel_180_240(colour)
    case colour
    when :red
      @s
    when :green
      (240 - @h) / 60 + (1 - (240 - @h) / 60) * @s
    else
      1
    end
  end

  def channel_240_300(colour)
    case colour
    when :red
      (@h - 240) / 60 + (1 - (@h - 240) / 60) * @s
    when :green
      @s
    else
      1
    end
  end

  def channel_300_360(colour)
    case colour
    when :green
      @s
    when :blue
      (360 - @h) / 60 + (1 - (360 - @h) / 60) * @s
    else
      1
    end
  end
end
