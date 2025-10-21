source 'https://rubygems.org'

gem "fastlane"

eval_gemfile("fastlane/Pluginfile")

# Add missing gems in ruby 3.4 until fastlane includes them
# https://github.com/fastlane/fastlane/issues/29183#issuecomment-2567093826
gem "abbrev"
gem "mutex_m"
gem "ostruct"

# Pull in openssl to avoid https://github.com/fastlane/fastlane/issues/29710
gem "openssl", ">= 3.3.1"
