#
#  Be sure to run `pod spec lint SETravel.podspec.podspec' to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see https://guides.cocoapods.org/syntax/podspec.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |spec|

  spec.name         = "SETravel.podspec"
  spec.version      = "1.0.0"
  spec.summary      = "A short description of SETravel."
  spec.description  = <<-DESC
                   DESC

  spec.homepage     = "https://github.com/SamChenzx/libSETravel"

  spec.license      = "MIT"
  # spec.license      = { :type => "MIT", :file => "FILE_LICENSE" }

  spec.author             = { "SamChenzx" => "samchen.swift@gmail.com" }
  
  spec.platform     = :ios
  # spec.platform     = :ios, "5.0"

  spec.ios.deployment_target = "10.0"

  spec.source       = { :git => "https://github.com/SamChenzx/libSETravel.git", :tag => "#{spec.version}" }
  spec.ios.vendored_libraries =  'libSETravel.a' 
  spec.source_files = '*.h','include/*.h'
  spec.public_header_files = '*.h','include/*.h'

  spec.frameworks = 'UIKit', 'MapKit'

end
